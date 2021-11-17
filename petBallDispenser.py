from flask import Flask, render_template
import pyodbc
import pandas as pd

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/Mechanical")
def mechanical():
    return render_template("mechanical.html")

@app.route("/Electrical")
def electrical():
    return render_template("electrical.html")

@app.route("/Firmware")
def firmware():
    return render_template("firmware.html")

@app.route("/About")
def about():
    return render_template("about.html")

def create_server_connection(ServerName, DatabaseName):
    connection = None
    try:
        connection = pyodbc.connect('Driver={SQL Server};'
                      'Server=ServerName;'
                      'Database=DatabaseName;'
                      'Trusted_Connection=yes;')
        print("MySQL Database connection successful")
    except pyodbc.Error as err:
        print(f"Error: '{err}'")

    return connection

if __name__ == '__main__':
   app.run(debug=True)