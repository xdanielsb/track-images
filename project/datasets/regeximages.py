import re
import subprocess as s
pattern = r'(http:\/\/[^"]*?\.(jpg|png))'
ffile = open("page.html", "r")
content = ffile.read()
#print(content)
pattern = re.compile(pattern)
for urlfile, ext in re.findall(pattern, content):
    #print(urlfile)
    cmd = "wget "+urlfile
    print(cmd)

