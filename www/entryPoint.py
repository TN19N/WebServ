from os import environ

cookies = environ["HTTP_COOKIE"].split(";")
for cookie in cookies:
    if cookie.find("code=123") != -1:
        print("Location: /app/", end="\r\n")
        print(end="\r\n")
        exit()

print("Content-Type: text/html", end="\r\n")
print("Set-Cookie: code=123", end="\r\n")
print(end="\r\n")

print("""
<!DOCTYPE html>
<html>
    <head>
        <title>Python</title>
    </head>
    <body>
        <h1>First Time</h1>
        <form action="/app/" method="GET">
            <input type="submit" value="Go to /app">
        </form>
    </body>
</html>
""")