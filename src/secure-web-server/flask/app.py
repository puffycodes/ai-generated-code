import os
import logging
from flask import Flask, render_template, request, redirect, url_for, session, flash
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address
from flask_wtf import FlaskForm
from flask_wtf.csrf import CSRFProtect
from wtforms import StringField, TextAreaField, validators
from wtforms.validators import Email, Length
import secrets

app = Flask(__name__)
app.secret_key = os.environ.get('SECRET_KEY', secrets.token_hex(32))

# Security setup
csrf = CSRFProtect(app)
limiter = Limiter(
    app,
    key_func=get_remote_address,
    default_limits=["200 per day", "50 per hour"]
)

# Logging setup
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class MessageForm(FlaskForm):
    email = StringField('Email', [Email(), Length(min=4, max=100)])
    message = TextAreaField('Message', [Length(min=1, max=500)])

@app.route('/')
def home():
    logger.info(f"Home page accessed from {request.remote_addr}")
    return render_template('home.html')

@app.route('/submit', methods=['GET', 'POST'])
@limiter.limit("10 per minute")
def submit():
    form = MessageForm()
    
    if request.method == 'POST':
        if form.validate_on_submit():
            email = form.email.data
            message = form.message.data
            
            logger.info(f"Message submitted from {request.remote_addr} with email: {email}")
            
            session['submitted_email'] = email
            session['submitted_message'] = message
            
            return redirect(url_for('result'))
        else:
            logger.warning(f"Invalid form submission from {request.remote_addr}")
            flash('Please correct the errors below.')
    
    return render_template('submit.html', form=form)

@app.route('/result')
def result():
    email = session.get('submitted_email')
    message = session.get('submitted_message')
    
    if not email or not message:
        flash('No submission found.')
        return redirect(url_for('submit'))
    
    # Clear session data after displaying
    session.pop('submitted_email', None)
    session.pop('submitted_message', None)
    
    return render_template('result.html', email=email, message=message)

@app.errorhandler(404)
def not_found(error):
    logger.warning(f"404 error from {request.remote_addr} for {request.url}")
    return render_template('error.html', error="Page not found"), 404

@app.errorhandler(500)
def internal_error(error):
    logger.error(f"500 error from {request.remote_addr}: {str(error)}")
    return render_template('error.html', error="Internal server error"), 500

if __name__ == '__main__':
    app.run(debug=False, host='127.0.0.1', port=5000)
