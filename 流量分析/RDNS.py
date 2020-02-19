
from dns import resolver

 

ans = resolver.query("www.baidu.com", "A")

print("qname:",ans.qname,"\n")

print ("reclass:",ans.rdclass,"\n")

print ("rdtype:",ans.rdtype,"\n")

print ("rrset:",ans.rrset,"\n")

print ("response:",ans.response,"\n")
