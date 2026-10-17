"""Download the official, version-pinned Editor using verified byte ranges."""
import concurrent.futures, pathlib, requests, time, hashlib, json
URL='https://download.unity3d.com/download_unity/cf0352b38e81/Windows64EditorInstaller/UnitySetup64-6000.3.17f1.exe'
OUT=pathlib.Path('logs/high_reuse/components')
SIZE=4162718136
CHUNK=32*1024*1024
def fetch(i):
    start=i*CHUNK;end=min(SIZE,start+CHUNK)-1
    p=OUT/'download-parts'/str(i)
    if p.exists() and p.stat().st_size==end-start+1:return
    for attempt in range(5):
        try:
            with requests.get(URL,headers={'Range':f'bytes={start}-{end}'},stream=True,timeout=(30,120)) as r:
                r.raise_for_status()
                assert r.status_code==206 and r.headers['Content-Range']==f'bytes {start}-{end}/{SIZE}',r.headers
                with p.open('wb') as f:
                    for data in r.iter_content(1024*1024):f.write(data)
            assert p.stat().st_size==end-start+1
            print('Downloaded',i,flush=True);return
        except Exception:
            if attempt==4:raise
            time.sleep(2)
def main():
    (OUT/'download-parts').mkdir(parents=True,exist_ok=True)
    count=(SIZE+CHUNK-1)//CHUNK
    with concurrent.futures.ThreadPoolExecutor(max_workers=16) as pool:list(pool.map(fetch,range(count)))
    dest=OUT/'UnitySetup64-6000.3.17f1.exe';sha=hashlib.sha256()
    with dest.open('wb') as f:
        for i in range(count):
            with (OUT/'download-parts'/str(i)).open('rb') as p:
                while data:=p.read(1024*1024):f.write(data);sha.update(data)
    (OUT/'editor-download.json').write_text(json.dumps(dict(url=URL,size=SIZE,sha256=sha.hexdigest()),indent=2))
    print('COMPLETE',sha.hexdigest(),flush=True)
if __name__=='__main__':main()
