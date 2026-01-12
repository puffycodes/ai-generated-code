package main

import (
	"html/template"
	"net/http"
)

type PageData struct {
	Email   string
	Message string
}

func main() {
	http.HandleFunc("/", homeHandler)
	http.HandleFunc("/submit", submitHandler)
	http.HandleFunc("/result", resultHandler)
	http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static/"))))
	
	http.ListenAndServe(":8080", nil)
}

func homeHandler(w http.ResponseWriter, r *http.Request) {
	tmpl := `<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <h1>Hello. Welcome to the Home Page.</h1>
    <a href="/submit">Submit Message</a>
</body>
</html>`
	
	t, _ := template.New("home").Parse(tmpl)
	t.Execute(w, nil)
}

func submitHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method == "POST" {
		http.Redirect(w, r, "/result?email="+r.FormValue("email")+"&message="+r.FormValue("message"), http.StatusSeeOther)
		return
	}
	
	tmpl := `<!DOCTYPE html>
<html>
<head>
    <title>Submit Message</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <h1>Submit Message</h1>
    <form method="post">
        <label>Email: <input type="email" name="email" required></label><br>
        <label>Message: <textarea name="message" required></textarea></label><br>
        <button type="submit">Submit</button>
    </form>
    <a href="/">Home</a>
</body>
</html>`
	
	t, _ := template.New("submit").Parse(tmpl)
	t.Execute(w, nil)
}

func resultHandler(w http.ResponseWriter, r *http.Request) {
	data := PageData{
		Email:   r.URL.Query().Get("email"),
		Message: r.URL.Query().Get("message"),
	}
	
	tmpl := `<!DOCTYPE html>
<html>
<head>
    <title>Result</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <h1>Message Submitted</h1>
    <p>Email: {{.Email}}</p>
    <p>Message: {{.Message}}</p>
    <a href="/submit">Submit Another Message</a> | <a href="/">Home</a>
</body>
</html>`
	
	t, _ := template.New("result").Parse(tmpl)
	t.Execute(w, data)
}
