# malicious-packet-checking
Malicious Packet Checking

Values (rounded, but calclated using graphing calculator)
---------------
m1 = -k*n / ln(1 - P^(1/k))
   = (-8 * 10000) / ln(1 - .0001^(1/8))
   = 210454.09

m2 = -k*n / ln(1 - P^(1/k))
   = (-8 * 10000) / ln(1 - .001^(1/8))
   = 146076.97

1,040,000 = 13 character * 8 bytes * 10,000 IPS

c1 = 1,040,000 / m1
   = 4.94
c2 = 1,040,000 / m2
   = 7.12
