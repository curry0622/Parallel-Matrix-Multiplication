def colored_text(text, color):
    try:
        import termcolor
        print(termcolor.colored(text, color))
    except ModuleNotFoundError:
        print(text)
