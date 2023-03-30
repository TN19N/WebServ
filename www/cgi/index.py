import cgi, sys
from time import sleep
from os import environ

print("Content-Type: text/html", end="\r\n")
print("Set-Cookie: 111111111", end="\r\n")
print("Set-Cookie: 222222222", end="\r\n")
print("Set-Cookie: 333333333", end="\r\n")


print("\r\n")

print(""" <style> @import 'https://fonts.googleapis.com/css?family=Inconsolata';
body { box-sizing: border-box; height: 100%; background-color: #000000;
background-image: radial-gradient(#11581E, #041607), url("https://media.giphy.com/media/oEI9uBYSzLpBK/giphy.gif");
background-repeat: no-repeat; background-size: cover; font-family: 'Inconsolata', Helvetica, sans-serif;
font-size: 1.5rem; color: rgba(128, 255, 128, 0.8); text-shadow: 0 0 1ex rgba(51, 255, 51, 1), 0 0 2px rgba(255, 255, 255, 0.8);}
    </style> """)

print("""
<body>
<h1> Hello From Cgi !!!!</h1>
 --------------------------------------
""")

for key, value in environ.items():
    print(f'<h5>{key}: {value}</h5>')

sleep(5)

for _ in range(1000):
    print('j' * 6000)


while True:
    line = sys.stdin.read(1337)
    if not line:
        break
    print("line lenght: ", len(line))
    # print(line)


print("</body>")
