from flask import Flask, request, jsonify, render_template, session
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address
from flask_talisman import Talisman
import secrets
import logging
import re
import html
import os

app = Flask(__name__, static_folder='build/static', template_folder='build')
app.secret_key = os.environ.get('SECRET_KEY', secrets.token_hex(32))

# Security headers
Talisman(app, force_https=False)

# Rate limiting
limiter = Limiter(
    app,
    key_func=get_remote_address,
    default_limits=["100 per hour"]
)

# Logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

def validate_email(email):
    pattern = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
    return re.match(pattern, email) is not None

def sanitize_input(text):
    return html.escape(text.strip())

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/api/submit', methods=['POST'])
@limiter.limit("10 per minute")
def submit_message():
    try:
        data = request.get_json()
        
        if not data or 'email' not in data or 'message' not in data:
            logger.warning(f"Invalid request from {request.remote_addr}")
            return jsonify({'error': 'Email and message required'}), 400
        
        email = sanitize_input(data['email'])
        message = sanitize_input(data['message'])
        
        if not validate_email(email):
            return jsonify({'error': 'Invalid email format'}), 400
        
        if len(message) > 1000:
            return jsonify({'error': 'Message too long'}), 400
        
        # Store in session for display
        session['last_submission'] = {'email': email, 'message': message}
        
        logger.info(f"Message submitted from {email}")
        return jsonify({'email': email, 'message': message})
        
    except Exception as e:
        logger.error(f"Error processing submission: {str(e)}")
        return jsonify({'error': 'Internal server error'}), 500

if __name__ == '__main__':
    app.run(debug=False, host='127.0.0.1', port=5000)
