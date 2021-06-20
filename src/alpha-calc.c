#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>

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
                digit = str[i] - 48;

                break;
            }

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            {
                digit = str[i] - 55;

                break;
            }

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            {
                digit = str[i] - 87;

                break;
            }

            default:
            {
                error = 1;

                return 0;
            }
        }

        ret += digit * pow(16, 1 - i);
    }

    return ret;
}

uint8_t in_loop = 0;

uint8_t run_alpha(char* root_alpha, uint8_t* i)
{
    if(error == 1)
        return 0;

    uint8_t opcode;

    *i += 1;
    for(; *i < strlen(root_alpha); *i += 1)
    {
        if(root_alpha[*i] == ' ') {}
        else if(root_alpha[*i] == '#')
        {
            *i += 1;
            opcode = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
            *i += 1;

            break;
        } 
        else if(root_alpha[*i] == '$')
        {
            *i += 1;
            opcode = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
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
                            *i += 1;
                            addr = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            addr = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;

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
                            *i += 1;
                            val = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            val = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;

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
                            *i += 1;
                            exit_code = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            exit_code = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;

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
                            *i += 1;
                            val = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            val = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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
                            *i += 1;
                            val = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            val = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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
                            *i += 1;
                            b = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            b = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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
                            *i += 1;
                            a = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            a = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;

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
                            *i += 1;
                            b = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            b = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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
                            *i += 1;
                            b = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            b = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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
                            *i += 1;
                            a = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;

                            break;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            a = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;

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
                            *i += 1;
                            b = hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' });
                            *i += 1;
                        } 
                        else if(root_alpha[*i] == '$')
                        {
                            *i += 1;
                            b = mem[hexstr2byte((char[]){ root_alpha[*i], root_alpha[*i + 1], '\0' })];
                            *i += 1;
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

int main(int argc, char* argv[])
{
    memset(mem, 0, 255);

    char input[255];

    if(argc > 2)
    {
        fputs("Too many arguments!", stderr);
        fprintf(stderr, "Usage: '%s [OPTIONAL: -n || --no-cursor]'\n", argv[0]);
        exit(-1);
    }

    char* cur = ">";
    
    if(argc > 1)
    {
        if(strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--no-cursor") == 0)
        {
            cur = "";
        }
        else
        {
            fprintf(stderr, "Unknown argument '%s'\n", argv[1]);
            fprintf(stderr, "Usage: '%s [OPTIONAL: -n || --no-cursor]'\n", argv[0]);
            exit(-1);
        }
    }


    for(;;)
    {
        error = 0;

        printf("%s", cur);
        fgets(input, 255, stdin);

        uint8_t i;
        for(i = 0; i < strlen(input); i++)
        {
            if(error == 1)
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
