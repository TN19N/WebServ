import cgi
from os import environ

form = cgi.FieldStorage()

cookies = environ["HTTP_COOKIE"]

print("Content-Type: text/html", end="\r\n")
print("Set-Cookie: cookie1=1", end="\r\n")
print("Set-Cookie: cookie2=2", end="\r\n")
print(end="\r\n")

print(cookies)