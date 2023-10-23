# malicious-packet-checking
Malicious Packet Checking

Values
---------------
m1 = -k*n / ln(1 - P^(1/k))
   = (-8 * 10000) / ln(1 - .005^(1/8))
   = 110347

We know the space cost for a list is 13 bytes -- one space for each char. We know RBF are bit arrays of m bits (0 or 1). We are given each cell only takes up 1 bit of space. Therefore, we know that 13 bytes -> 13 bits = 