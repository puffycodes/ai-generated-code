from flask import Flask, request, jsonify, render_template
from flask_cors import CORS

app = Flask(__name__, static_folder='build', static_url_path='')
CORS(app)

@app.route('/')
def serve():
    return app.send_static_file('index.html')

@app.route('/api/submit', methods=['POST'])
def submit_message():
    data = request.get_json()
    email = data.get('email', '')
    message = data.get('message', '')
    
    return jsonify({
        'email': email,
        'message': message
    })

if __name__ == '__main__':
    app.run(debug=True)
