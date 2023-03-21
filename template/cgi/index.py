import cgi
from os import environ

form = cgi.FieldStorage()

print("Content-Type: text/html", end="\r\n")

print("""
<h1> Hello From Cgi !!!! </h1>
 --------------------------------------

""")

for key, value in environ.items():
    print(f'{key}:{value}', end="\r\n")