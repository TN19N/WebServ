from os import environ
import json
from cookieGenerator import generate_cookie

cookies = environ["HTTP_COOKIE"].split(";")
method = environ["REQUEST_METHOD"]
