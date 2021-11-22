import pyodbc

def create_server_connection():
    connection = None
    try:
        connection = pyodbc.connect('Driver={SQL Server};'
                      'Server=Localhost;'
                      'Database=SeniorDesign;'
                      'Trusted_Connection=yes;')
        print("SQL Database connection successful")
    except pyodbc.Error as err:
        print("Error: '{err}'")

    return connection

def getTest():
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM Reviews')
    for row in cursor:
        print('row = %r' % (row,))

def close_db(e=None):
    db = g.pop('db', None)

    if db is not None:
        db.close()