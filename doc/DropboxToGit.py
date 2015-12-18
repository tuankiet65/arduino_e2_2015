#!/usr/bin/env python3
import dropbox
import git
import hashlib

# No, I'm not leaking the token to you guys
TOKEN="XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx"
FOLDER="/E2_2015"

def getFolderContent(folderName):
	result=dbx.files_list_folder(folderName)
	content=[]
	for entry in result.entries:
		content.append(entry.name)
	return content

def getFileRevision(fileName):
	result=dbx.files_list_revisions(fileName, limit=100)
	revision=[]
	for entry in result.entries:
		revision.append({"name": entry.name, "id": entry.rev, "date": entry.server_modified, 'client_date': entry.client_modified})
	return revision

def getFileHash(fileName):
	h=hashlib.md5()
	with open(fileName, "rb") as f:
		h.update(f.read())
	return h.hexdigest()

dbx=dropbox.Dropbox(TOKEN)
repo=git.Repo('..')
fileHash=[]
revisions=[]
tmp=[]

print("Getting content in folder %s"%(FOLDER))
folderContent=getFolderContent(FOLDER)
for f in folderContent:
	print("- %s"%(f))

for f in folderContent:
	print("Getting revisions for file %s"%(f))
	tmp=getFileRevision("%s/%s"%(FOLDER, f))
	print("Got %d revisions for file %s"%(len(tmp), f))
	revisions.extend(tmp)

revisions.sort(key=lambda entry: entry["date"])

for entry in revisions:
	print("Downloaing file:%s rev:%s"%(entry['name'], entry['id']))
	dbx.files_download_to_file(entry['name'], "rev:%s"%(entry['id']))
	if not(getFileHash(entry['name']) in fileHash):
		repo.index.add(["doc/%s"%(entry['name'])])
		repo.git.commit(date=entry['date'].isoformat(), m="\"DropboxToGit: file:%s rev:%s client_modified:%s\""%(entry['name'], entry['id'], entry['client_date'].isoformat()))
		fileHash.append(getFileHash(entry['name']))

# Push by yourself please, because I'm lazy