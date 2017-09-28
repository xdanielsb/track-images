import re
#pattern = r'http?://[a-zA-Z0-9.]/[a-zA-Z0-9-&.]+\.(jpg|png|gif|tif|exf|svg|wfm)'
#pattern = r'(http:\/\/.*\.(?:png|jpg))'
#pattern = r"(http?:\/\/.*\.(?:png|jpg))"
pattern = r'(http:\/\/[^"]*?\.(jpg|png))'
ffile = open("page.html", "r")
content = ffile.read()
#print(content)
pattern = re.compile(pattern)
for vals in re.findall(pattern, content):
    print(vals[0])
