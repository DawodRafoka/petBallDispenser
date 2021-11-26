from flask import Flask, render_template, send_from_directory, abort, redirect, url_for,session, flash
from forms import ContactForm
import pyodbc
from werkzeug.utils import safe_join
from datetime import timedelta
 
app = Flask(__name__)
app.secret_key = 'db216e29fad21ff702244b6c0de749bf8a038b606daf19402e20d13899b2c002'    
app.config['PERMANENT_SESSION_LIFETIME'] =  timedelta(minutes=30)   
app.config['Parts'] =  "../flaskr/static/Parts"
conn = None
try:
    conn = pyodbc.connect('Driver={SQL Server};'
                    'Server=DESKTOP-8DKJ560;'
                    'Database=SeniorDesign;'
                    'Trusted_Connection=yes;')
    cursor = conn.cursor()
    print("SQL Database connection successful")
except pyodbc.Error as err:
    print("Error: '{err}'",err)
    exit()

@app.before_first_request
def StartSession():
    cursor.execute('SELECT * FROM Sessions')
    temp = max(cursor)
    # print(temp[0])
    session['SessionId'] = temp[0] + 1
    query = """INSERT INTO Sessions (SessionId) VALUES (?)"""
    args = (session['SessionId'])
    cursor.execute(query, args)
    cursor.execute('SELECT * FROM Sessions')
    # for row in cursor:
    #     print(row)
    # print("Session added")
    return redirect(url_for('index'))
    

@app.route("/", methods=['get', 'post'])
def index():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        # print("\nData received. ")
    return render_template("index.html", form=form)

@app.route("/Mechanical", methods=['get'])
def mechanical():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        # print("\nData received. ")
    return render_template("mechanical.html", form=form)

@app.route("/Electrical", methods=['get', 'post'])
def electrical():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        # print("\nData received. ")
    return render_template("electrical.html", form=form)

@app.route("/Firmware", methods=['get', 'post'])
def firmware():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        # print("\nData received. ")
    return render_template("firmware.html", form=form)

@app.route("/About", methods=['get', 'post'])
def about():
    form = ContactForm()
    if form.validate_on_submit():
        query = """INSERT INTO Reviews (SessionId, First_Name,Last_Name,Email,Review) VALUES(?,?,?,?,?)"""
        args = (session['SessionId'],form.firstName.data,form.lastName.data,form.email.data,form.comment.data)
        cursor.execute(query, args).commit()
        flash("Message Received", "success")
        # print("\nData received. ")
    return render_template("about.html", form=form)

@app.route("/<Folder>/<FileName>", methods=['get'])
def download(Folder, FileName):
    safe_path = safe_join(Folder, FileName)
    print(safe_path)
    if Folder == 'Mechanical':
        query = """INSERT INTO Mechanical (SessionId, Part_Name,Part_Path) VALUES(?,?,?)"""
        args = (session['SessionId'],FileName,safe_path)
        cursor.execute(query, args).commit()
        
    elif Folder == 'Electrical':
        query = """INSERT INTO Electrical (SessionId, Design_Name,Design_Path) VALUES(?,?,?)"""
        args = (session['SessionId'],FileName,safe_path)
        cursor.execute(query, args).commit()
        
    elif Folder == 'Firmware':
        query = """INSERT INTO Firmware (SessionId, Firmware_Name, Firmware_Path) VALUES(?,?,?)"""
        args = (session['SessionId'],FileName,safe_path)
        cursor.execute(query, args).commit()
    else:
        abort(404)        
    try:
        return send_from_directory(app.config["Parts"],safe_path)
    except FileNotFoundError:
        abort(404)


# def create_server_connection():
#     conn = None
#     try:
#         conn = pyodbc.connect('Driver={SQL Server};'
#                       'Server=JBUNNELLSTUDIO;'
#                       'Database=SeniorDesign;'
#                       'Trusted_Connection=yes;')
#         print("SQL Database connection successful")
#     except pyodbc.Error as err:
#         print("Error: '{err}'",err)
#         exit()
#     return conn
        
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