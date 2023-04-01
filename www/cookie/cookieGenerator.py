from string import ascii_letters as letters
from random import randint


def generate_cookie(strong: int = 15) -> str:
    size = len(letters) - 1
    cookie = [letters[randint(0, size)] for _ in range(strong)]
    return "".join(cookie)
