from os import path, listdir


def GetFiles(Folder):
    if not path.exists(Folder):
        return None
    Files = [f for f in listdir(Folder)
             if path.isfile(path.join(Folder, f))]
    return Files
