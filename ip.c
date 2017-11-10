#include <stdio.h>

// Mask used for extracting a byte from an integer
#define MASK 0xff

// Function used to convert an ip from bytes to integer
unsigned get_ip(unsigned a, unsigned b, unsigned c, unsigned d)
{
    return (a << 24) | (b << 16) | (c << 8) | (d << 0);
}

// Function used to convert netmask length to integer
unsigned get_netmask(int length)
{
    int pos;
    unsigned netmask = 0;

    // Put bits of 1 in mask until the length is reached
    for (pos = 0; pos < length; pos++)
    {
        netmask <<= 1;
        netmask |= 1;
    }

    // Shift mask to left until max length is reached
    for (pos = 0; pos < 32 - length; pos++)
        netmask <<= 1;

    return netmask;
}

// Function used to print an ip in binary format
void print_ip_binary(unsigned ip)
{
    int pos;

    // Extract each bit from left to right and print it
    for (pos = 31; pos >= 0; pos--)
    {
        unsigned char bit = (ip >> pos) & 1;
        printf("%u", bit);

        // Print a dot after each byte, except the last one
        if (pos % 8 == 0 && pos != 0)
            printf(".");
    }
}

// Function used to print a number in base 32
void print_32number(unsigned number)
{
    // Stop condition
    if (number == 0)
        return;

    // Make a recursive call because we need the remainders in inverse order
    print_32number(number / 32);

    // Obtain the remainder
    unsigned mod = number % 32;

    // If remainder is lower than 10 print it as it is
    if (mod < 10)
        printf("%d", mod);
    // If the remainder in greather than 10 print the coresponding letter
    else
        printf("%c", mod - 10 + 65);
}

// Function used to print ip in base 32
void print_ip_in_base32(unsigned ip)
{
    int i;

    // Print each byte of ip in base 32
    for (i = 3; i >= 0; i--)
    {
        unsigned byte = (ip >> (i * 8)) & MASK;

        if (byte == 0)
            printf("0");
        print_32number(byte);

        // Add a dot only after the first 3 bytes
        if (i != 0)
            printf(".");
    }
}

// Function used to parse a input line
void parse_line()
{
    unsigned a, b, c, d;
    int len;

    // Read IP_1 and NETMASK length from stdin
    scanf("%u.%u.%u.%u/%d", &a, &b, &c, &d, &len);

    // Convert IP_1 and NETMASK in integers
    unsigned ip1 = get_ip(a, b, c, d);
    unsigned netmask = get_netmask(len);

    // Obtain network and brodcast addresses for IP_1
    unsigned network = ip1 & netmask;
    unsigned broadcast = ip1 | ~netmask;

    // Read IP_2 from stdin
    scanf("%u.%u.%u.%u", &a, &b, &c, &d);

    // Convert IP_2 in integer
    unsigned ip2 = get_ip(a, b, c, d);

    // Print IP_1 in decimal format
    printf("-0 %u.%u.%u.%u\n", (ip1 >> 24) & MASK,
                               (ip1 >> 16) & MASK,
                               (ip1 >>  8) & MASK,
                               (ip1 >>  0) & MASK);

    // Print NETMASK in decimal format
    printf("-1 %u.%u.%u.%u\n", (netmask >> 24) & MASK,
                               (netmask >> 16) & MASK,
                               (netmask >>  8) & MASK,
                               (netmask >>  0) & MASK);

    // Print IP_1 network address in decimal format
    printf("-2 %u.%u.%u.%u\n", (network >> 24) & MASK,
                               (network >> 16) & MASK,
                               (network >>  8) & MASK,
                               (network >>  0) & MASK);

    // Print IP_1 broadcast address in decimal format
    printf("-3 %u.%u.%u.%u\n", (broadcast >> 24) & MASK,
                               (broadcast >> 16) & MASK,
                               (broadcast >>  8) & MASK,
                               (broadcast >>  0) & MASK);

    // Print 'da' if IP_1 and IP_2 are in the same network or 'nu' otherwise
    int same_network = (ip1 & netmask) == (ip2 & netmask);
    printf("-4 %s\n", same_network ? "da" : "nu");


    // Print IP_1 in hexadecimal format and IP_2 in octal format
    printf("-5 %X.%X.%X.%X %o.%o.%o.%o\n", (ip1 >> 24) & MASK,
                                           (ip1 >> 16) & MASK,
                                           (ip1 >>  8) & MASK,
                                           (ip1 >>  0) & MASK,
                                           (ip2 >> 24) & MASK,
                                           (ip2 >> 16) & MASK,
                                           (ip2 >>  8) & MASK,
                                           (ip2 >>  0) & MASK);

    printf("-6");

    // If IP_1 and IP_2 are in the same network print '0'
    if (same_network)
        printf(" 0");

    int nr, index;

    // Read the number of IPs
    scanf("%d", &nr);

    // For each IP in list
    for (index = 1; index <= nr; index++)
    {
        // Read IP and NETMASK
        scanf("%d.%d.%d.%d/%d", &a, &b, &c, &d, &len);

        // Compute results only if IP_1 and IP_2 are not in the same network
        if (!same_network)
        {
            // Convert IP and NETMASK in integers
            unsigned ip = get_ip(a, b, c, d);
            unsigned ip_netmask = get_netmask(len);

            // Check if IP and IP_2 are in the same network and print the index
            if ((ip2 & ip_netmask) == (ip & ip_netmask) && !same_network)
                printf(" %d", index);
        }
    }

    printf("\n");

    // Print IP_1 and IP_2 in binary format
    printf("-7 ");
    print_ip_binary(ip1);
    printf(" ");
    print_ip_binary(ip2);
    printf("\n");

    // Print IP_1 and IP_2 in base 32 format
    printf("-8 ");
    print_ip_in_base32(ip1);
    printf(" ");
    print_ip_in_base32(ip2);
    printf("\n");
}


int main()
{
    int nr_lines, line;

    // Read the number of lines
    scanf("%d", &nr_lines);

    // For each line parse the input and solve tasks also print the line number
    for (line = 1; line <= nr_lines; line++)
    {
        printf("%d\n", line);
        parse_line();
    }

    return 0;
}

