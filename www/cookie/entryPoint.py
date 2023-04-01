from os import environ
import json
from cookieGenerator import generate_cookie

cookies = environ["HTTP_COOKIE"].split(";")
method = environ["REQUEST_METHOD"]
new_cookie = generate_cookie()

print("Set-Cookie: session=", new_cookie,sep="", end="\r\n")

print("\r\n")

print("Your Old Cookie: ", end="")
[print(cookie) for cookie in cookies if cookie[0:8] == "session="]

print("\nYour New Cookie: session=", new_cookie, sep="")
