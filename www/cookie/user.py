

def print_dashboard(user: str) -> None:
    print("""<!DOCTYPE html>
<html>
<head>
<style>
body {background-color: powderblue;}
h1   {color: blue;}
p    {color: red;}
</style>
</head>
<body>
    """)

    print(f"""
<h1>Hello {user}</h1>

</body>
</html>""")

def print_login() -> None:
    pass