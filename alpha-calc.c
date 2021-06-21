#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

uint8_t power(uint8_t base, uint8_t pow)
{
    uint8_t ret = 1;
    for(; pow > 0; pow--)
        ret *= base;

    return ret;
}

uint8_t mem[255];
uint8_t error = 0;

uint8_t hexstr2byte(char* str)
{
    uint8_t i = 0;
    uint8_t ret = 0;

    if(strlen(str) > 2)
        return 0;

    for(; i < strlen(str); i += 1)
    {
        uint8_t digit;

        switch(str[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                digit = str[i] - '0';

                break;
            }

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            {
                digit = str[i] - ('A' - 10);

                break;
            }

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            {
                digit = str[i] - ('a' - 10);

                break;
            }

            default:
            {
                error = 1;

                return 0;
            }
        }

        ret += digit * power(16, 1 - i);
    }

    return ret;
}

uint8_t get_ctrl_char(uint8_t ch)
{
    switch(ch)
    {
        case 'a':
            return '\a';

        case 'b':
            return '\b';

        case 'f':
            return '\f';

        case 'n':
            return '\n';

        case 'r':
            return '\r';

        case 't':
            return '\t';

        case 'v':
            return '\v';

        case '0':
            return '\0';

        case '$':
            return '$';
    }

    return 0;
}

uint8_t in_loop = 0;

uint8_t run_alpha(char* root_alpha, uint8_t* i)
{
    uint8_t opcode;

    if(error)
        return 0;

    *i += 1;
    for(; *i < strlen(root_alpha); *i += 1)
    {
        if(root_alpha[*i] == ' ') {}
        else if(root_alpha[*i] == '#')
        {
            char num[3];
            *i += 1;
            num[0] = root_alpha[*i];
            num[1] = root_alpha[*i + 1];
            num[2] = '\0';
            opcode = hexstr2byte((char*)num);
            *i += 1;

            break;
        } 
        else if(root_alpha[*i] == '$')
        {
            char num[3];
            *i += 1;
            num[0] = root_alpha[*i];
            num[1] = root_alpha[*i + 1];
            num[2] = '\0';
            opcode = mem[hexstr2byte((char*)num)];
            *i += 1;

            break;
        }
        else
        {
            fprintf(stderr, "error:%d: expected value or address\n", *i);

            error = 1;
            return 0;
        }
    }

    *i += 1;
    for(; *i < strlen(root_alpha); *i += 1)
    {
        if(root_alpha[*i] == ' ')
        {
            continue;
        }
        else if(root_alpha[*i] == '(')
        {
            *i += 1;

            switch(opcode)
            {
                case 0x00: /* store */
                {
                    uint8_t addr, val;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            addr = hexstr2byte((char*)num);
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            addr = mem[hexstr2byte((char*)num)];
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                addr = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                addr = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }

                            break;
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            addr = run_alpha(root_alpha, i);
                            *i += 1;

                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }
                    }

                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ',')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ','\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;
                    
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = hexstr2byte((char*)num);
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = mem[hexstr2byte((char*)num)];
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                val = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                val = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }

                            break;
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            val = run_alpha(root_alpha, i);
                            *i += 1;

                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }
                    }

                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ')')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ')'\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    mem[addr] = val;

                    return 0;
                }

                case 0x01: /* exit */
                {
                    uint8_t exit_code;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            exit_code = hexstr2byte((char*)num);
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            exit_code = mem[hexstr2byte((char*)num)];
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                exit_code = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                exit_code = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }

                            break;
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            exit_code = run_alpha(root_alpha, i);
                            *i += 1;

                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }
                    }

                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ')')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ')'\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    exit(exit_code);

                    return 0;
                }

                case 0x02: /* putint */
                {
                    uint8_t val;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                val = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                val = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            val = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;
                        
                        if(root_alpha[*i] == ')')
                        {
                            printf("%d", val);

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        printf("%d", val);
                    }
                    *i += 1;

                    return 0;
                }

                case 0x03: /* putchar */
                {
                    uint8_t val;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            val = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                val = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                val = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            val = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;
                        
                        if(root_alpha[*i] == ')')
                        {
                            printf("%c", val);

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        printf("%c", (char)val);
                    }
                    *i += 1;

                    return 0;
                }

                case 0x04: /* add */
                {
                    uint8_t a = 0, b;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                b = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                b = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            b = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;
                        
                        if(root_alpha[*i] == ')')
                        {
                            a += b;

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        a += b;
                    }
                    *i += 1;

                    return a;
                }

                case 0x05: /* sub */
                {
                    uint8_t a, b;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            a = hexstr2byte((char*)num);
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            a = mem[hexstr2byte((char*)num)];
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                a = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                a = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }

                            break;
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            a = run_alpha(root_alpha, i);
                            *i += 1;

                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }
                    }

                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ',')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ','\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                b = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                b = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            b = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;
                        
                        if(root_alpha[*i] == ')')
                        {
                            a -= b;

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        a -= b;
                    }
                    *i += 1;

                    return a;
                }

                case 0x06: /* mul */
                {
                    uint8_t a = 1, b;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                b = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                b = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            b = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;
                        
                        if(root_alpha[*i] == ')')
                        {
                            a *= b;

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        a *= b;
                    }
                    *i += 1;

                    return a;
                }

                case 0x07: /* div */
                {
                    uint8_t a, b;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            a = hexstr2byte((char*)num);
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            a = mem[hexstr2byte((char*)num)];
                            *i += 1;

                            break;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                a = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                a = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }

                            break;
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            a = run_alpha(root_alpha, i);
                            *i += 1;

                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }
                    }

                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ',')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ','\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == '#')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = hexstr2byte((char*)num);
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            char num[3];
                            *i += 1;
                            num[0] = root_alpha[*i];
                            num[1] = root_alpha[*i + 1];
                            num[2] = '\0';
                            b = mem[hexstr2byte((char*)num)];
                            *i += 1;
                        }
                        else if(root_alpha[*i] == '\'')
                        {
                            *i += 1;
                            if(root_alpha[*i] != '$')
                            {
                                b = root_alpha[*i];
                            }
                            else
                            {
                                *i += 1;
                                b = get_ctrl_char(root_alpha[*i]);
                            }

                            *i += 1;
                            if(root_alpha[*i] != '\'')
                            {
                                fprintf(stderr, "error:%d: expected character '''\n", *i);

                                error = 1;
                                return 0;
                            }
                        }
                        else if(root_alpha[*i] == 'a')
                        {
                            b = run_alpha(root_alpha, i);
                            *i += 1;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected value, address or alpha\n", *i);

                            error = 1;
                            return 0;
                        }

                        *i += 1;

                        if(b == 0)
                        {
                            fprintf(stderr, "error:%d: division by zero gives undefined result\n", *i);

                            error = 1;
                            return 0;
                        }
                        
                        if(root_alpha[*i] == ')')
                        {
                            a /= b;

                            break;
                        }
                        else
                        {
                            for(; *i < strlen(root_alpha); *i += 1)
                            {
                                if(root_alpha[*i] == ' ') {}
                                else if(root_alpha[*i] == ',')
                                {
                                    break;
                                }
                                else
                                {
                                    fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        a /= b;
                    }
                    *i += 1;

                    return a;
                }

                case 0x08: /* loop */
                {
                    int first = *i, last;

                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ')')
                        {
                            last = *i;
                            break;
                        }
                    }

                    in_loop = 1;
                    for(;in_loop;)
                    {
                        *i = first;

                        for(; *i < strlen(root_alpha); *i += 1)
                        {
                            if(error)
                            {
                                in_loop = 0;
                                break;
                            }

                            if(root_alpha[*i] == 'a')
                            {
                                run_alpha(root_alpha, i);
                                *i += 1;
                            }
                            else
                            {
                                fprintf(stderr, "error:%d: expected alpha\n", *i);

                                error = 1;
                                return 0;
                            }

                            *i += 1;

                            if(root_alpha[*i] == ')')
                            {
                                break;
                            }
                            else
                            {
                                for(; *i < strlen(root_alpha); *i += 1)
                                {
                                    if(root_alpha[*i] == ' ') {}
                                    else if(root_alpha[*i] == ',')
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        fprintf(stderr, "error:%d: expected character ')'\n", *i);

                                        error = 1;
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    *i += 1;

                    return 0;
                }

                case 0x09: /* break */
                {
                    *i += 1;
                    for(; *i < strlen(root_alpha); *i += 1)
                    {
                        if(root_alpha[*i] == ' ') {}
                        else if(root_alpha[*i] == ')')
                        {
                            break;
                        }
                        else
                        {
                            fprintf(stderr, "error:%d: expected character ')'\n", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    if(in_loop)
                    {
                        in_loop = 0;
                    }
                    else
                    {
                        fprintf(stderr, "error:%d: cannot break outside a loop", *i);

                        error = 1;
                        return 0;
                    }

                    return 0;
                }
            }
        }
        else
        {
            fprintf(stderr, "error:%d: expected character '('\n", *i);

            error = 1;
            return 0;
        }
    }

    return 0; 
}

int main()
{
    char input[255];
    
    memset(mem, 0, 255);

    for(;;)
    {
        uint8_t i;
        error = 0;

        printf(">");
        fgets(input, 255, stdin);

        for(i = 0; i < strlen(input); i++)
        {
            if(error)
                break;

            switch(input[i])
            {
                case ' ':
                case '\n':
                    break;

                case 'a':
                {
                    run_alpha(input, &i);
                } break;

                default:
                    break;
            }
        }
    }

    return 0;
}
