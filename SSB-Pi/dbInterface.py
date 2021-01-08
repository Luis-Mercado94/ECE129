import sqlite3
import random

#initializer functions
def connect_db(dbname):
    return sqlite3.connect(dbname + '.db')

def connect_cursor(conn):
    return conn.cursor()

def create_table(obj):
    try:
        obj.c.execute('CREATE TABLE IF NOT EXISTS table1(barcode TEXT, weight REAL, cvType TEXT, status TEXT)') 
        return True
    except:
        print("Error making the table!")
        return False
    
def close_conn(obj):
    obj.c.close()
    obj.conn.close()

#database entry functions
def full_entry_keyboard(obj):
    bar = input_barcode()
    wt = input_weight()
    cv = input_cv()
    stat = input_recy_stat()
    obj.c.execute("INSERT INTO table1 (barcode, weight, cvType, status) VALUES (?, ?, ?, ?)", (bar, wt, cv, stat))
    obj.c.execute("INSERT INTO table1 (barcode, weight, cvType) VALUES (?, ?, ?)", (bar, wt, cv))
    obj.conn.commit()

# database entry with the barcode scanner
def full_entry_scanner(obj):
    bar = obj.bc
    wt = obj.itemWeight
    cv = obj.itemType
    stat = input_recy_stat()
#     obj.c.execute("INSERT INTO table1 (barcode, weight, cvType, status) VALUES (?, ?, ?, ?)", (bar, wt, cv, stat))
    obj.c.execute("INSERT INTO table1 (barcode, weight, cvType) VALUES (?, ?, ?)", (bar, wt, cv))
    obj.conn.commit()
        
def part_entry(conn, c):
    bar = 'N/A'
    wt = input_weight()
    cv = input_cv()
    stat = input_recy_stat()
    c.execute("INSERT INTO table1 (barcode, weight, cvType, status) VALUES (?, ?, ?, ?)",
    (bar, wt, cv, stat))
    conn.commit()
    
def read_entry(obj):
    bc = obj.bc
    obj.c.execute("SELECT * FROM table1 WHERE barcode LIKE %s" %(bc))
    data = obj.c.fetchone()
    print(data)
    if(data != None):
        obj.bcType = data[1]
        obj.bcWeight = data[2]
        return obj
    else:
        obj.bcType = "UNK"
        obj.bcWeight = 0
        obj.newBC = True
        return obj
    

#def edit_entry(## search parameter and new data ##)
    

#weight averaging functions
#def count_barcodes(barcode)
    #this function will search the .db for the number of instances of a barcode
    
#def average_weights(barcode)
    #this will return the mean weight of a barcoded item
    #doing this will require a way of filtering outliers in the Gaussian distribution
   
#data collection helper functions
def input_barcode():
    return input('Please scan barcode now: ')

def input_weight():
    #random input is a placeholder
    #function should call the weight sensor
    return random.randrange(1,20)

def input_cv():
    #random input is a placeholder
    #function should call the cv, consult software for appropriate data type
    type = random.randint(1,4)
    if type == 1:
        return 'Aluminum'
    if type == 2:
        return 'Plastic'
    if type == 3:
        return 'Glass'
    return 'Trash'
    
def input_recy_stat():
    #random input is a placeholder
    #function needs more complexity TBD by algorithm
    status = random.randint(1,3)
    if status == 1:
        return 'recyclable'
    if status == 2:
        return 'non-recyclable'
    if status == 3:
        return 'questionable'