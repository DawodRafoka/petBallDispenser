# petBallDispenser
## Setup
### Requirements
To set up demo website please download the contents of the flaskr folder and run the follwing command:

 pip install -r requirements.txt
 
 Once all required packages are downloaded move on to Database setup. 
 ### Database 
 Download the SeniorDesign.bak file in the Database backup folder, this is the MSSQLSever schema nesscary to run our website. Once that is downloaded
 open and import the backup via MS SQL server managemet studio. Once that backup is restored navigate to Petballdispenser.py and change the following
 line of code to the correct name of your system. 
 
     conn = pyodbc.connect('Driver={SQL Server};'
                    'Server=<YourPCName>;'
                    'Database=SeniorDesign;'
                    'Trusted_Connection=yes;')
                    
  Save the file and proceed to run the flaks app.
 
 ### Run
 Navigate into the flaskr folder and use this run command:
 
 python .\petBallDispenser.py
 
 After the application is finished preparing control click on the local host link in the terminal window to navigate to the page.
