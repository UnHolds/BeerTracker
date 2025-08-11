import subprocess

revision = (
    subprocess.check_output(["git", "log", "-1", "--format=%h"])
    .strip()
    .decode("utf-8")
)
print("'-D COMMIT_HASH=\"%s\"'" % revision)
