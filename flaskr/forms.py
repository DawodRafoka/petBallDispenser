from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, TextAreaField
from wtforms.validators import DataRequired, Email

class ContactForm(FlaskForm):
    firstName = StringField("First Name: ", validators=[DataRequired()])
    lastName = StringField("Last Name: ", validators=[DataRequired()])
    email = StringField("Email: ", validators=[Email()])
    comment = TextAreaField("Comments: ", validators=[DataRequired()])
    submit = SubmitField("Submit")