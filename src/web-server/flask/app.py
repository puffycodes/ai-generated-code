from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/submit')
def submit_form():
    return render_template('submit.html')

@app.route('/process', methods=['POST'])
def process_submission():
    email = request.form['email']
    message = request.form['message']
    return render_template('result.html', email=email, message=message)

if __name__ == '__main__':
    app.run(debug=True)
