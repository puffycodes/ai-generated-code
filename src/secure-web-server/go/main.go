package main

import (
	"html/template"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/csrf"
	"github.com/gorilla/mux"
	"github.com/gorilla/sessions"
	"golang.org/x/time/rate"
)

var (
	store     = sessions.NewCookieStore([]byte(os.Getenv("SESSION_KEY")))
	templates = template.Must(template.ParseGlob("templates/*.html"))
	limiter   = rate.NewLimiter(rate.Every(time.Second), 10)
)

type Message struct {
	Email   string
	Content string
}

func rateLimitMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if !limiter.Allow() {
			http.Error(w, "Rate limit exceeded", http.StatusTooManyRequests)
			return
		}
		next.ServeHTTP(w, r)
	})
}

func homeHandler(w http.ResponseWriter, r *http.Request) {
	templates.ExecuteTemplate(w, "home.html", nil)
}

func submitHandler(w http.ResponseWriter, r *http.Request) {
	templates.ExecuteTemplate(w, "submit.html", map[string]interface{}{
		csrf.TemplateTag: csrf.TemplateField(r),
	})
}

func processHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != "POST" {
		http.Redirect(w, r, "/", http.StatusSeeOther)
		return
	}

	email := template.HTMLEscapeString(r.FormValue("email"))
	message := template.HTMLEscapeString(r.FormValue("message"))

	if email == "" || message == "" {
		http.Error(w, "Email and message are required", http.StatusBadRequest)
		return
	}

	msg := Message{Email: email, Content: message}
	templates.ExecuteTemplate(w, "result.html", msg)
}

func main() {
	if os.Getenv("SESSION_KEY") == "" {
		log.Fatal("SESSION_KEY environment variable is required")
	}

	r := mux.NewRouter()
	r.Use(rateLimitMiddleware)

	csrfMiddleware := csrf.Protect([]byte(os.Getenv("SESSION_KEY")))

	r.HandleFunc("/", homeHandler)
	r.HandleFunc("/submit", submitHandler)
	r.HandleFunc("/process", processHandler)
	r.PathPrefix("/static/").Handler(http.StripPrefix("/static/", http.FileServer(http.Dir("static/"))))

	log.Println("Server starting on :8080")
	log.Fatal(http.ListenAndServe(":8080", csrfMiddleware(r)))
}
