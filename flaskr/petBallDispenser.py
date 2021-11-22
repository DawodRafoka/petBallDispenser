from logging import Manager
from flask import Flask, render_template, request, redirect, url_for,session, flash
from forms import ContactForm
import pyodbc
import pandas as pd
from datetime import timedelta

app = Flask(__name__)
app.secret_key = 'db216e29fad21ff702244b6c0de749bf8a038b606daf19402e20d13899b2c002'    
app.config['PERMANENT_SESSION_LIFETIME'] =  timedelta(minutes=30)
conn = None
try:
    conn = pyodbc.connect('Driver={SQL Server};'
                    'Server=JBUNNELLSTUDIO;'
                    'Database=SeniorDesign;'
                    'Trusted_Connection=yes;')
    print("SQL Database connection successful")
except pyodbc.Error as err:
    print("Error: '{err}'",err)
    exit()
cursor = conn.cursor()
@app.before_first_request
def StartSession():
    cursor.execute('SELECT * FROM Sessions')
    temp = max(cursor)
    print(temp[0])
    session['SessionId'] = temp[0] + 1
    query = """INSERT INTO Sessions (SessionId) VALUES (?)"""
    args = (session['SessionId'])
    cursor.execute(query, args)
    cursor.execute('SELECT * FROM Sessions')
    for row in cursor:
        print(row)
    print("Session added")
    return redirect(url_for('index'))
    

@app.route("/", methods=['get', 'post'])
def index():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        print("\nData received. ")
    return render_template("index.html", form=form)

@app.route("/Mechanical", methods=['get', 'post'])
def mechanical():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        print("\nData received. ")
    return render_template("mechanical.html", form=form)

@app.route("/Electrical", methods=['get', 'post'])
def electrical():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        print("\nData received. ")
    return render_template("electrical.html", form=form)

@app.route("/Firmware", methods=['get', 'post'])
def firmware():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        print("\nData received. ")
    return render_template("firmware.html", form=form)

@app.route("/About", methods=['get', 'post'])
def about():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        print("\nData received. ")
    return render_template("about.html", form=form)

def create_server_connection():
    conn = None
    try:
        conn = pyodbc.connect('Driver={SQL Server};'
                      'Server=JBUNNELLSTUDIO;'
                      'Database=SeniorDesign;'
                      'Trusted_Connection=yes;')
        print("SQL Database connection successful")
    except pyodbc.Error as err:
        print("Error: '{err}'",err)
        exit()
    return conn
        
def getTest(conn):
    print("Get Test")
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM Reviews')
    for row in cursor:
        print(row)

def close_db(conn):
    if conn is not None:
        conn.close()
    


if __name__ == '__main__':
    app.run(debug=True)