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
            fprintf(stderr, "error:%d: expected value or address", *i);

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
                            fprintf(stderr, "error:%d: expected value, address or alpha", *i);

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
                            fprintf(stderr, "error:%d: expected character ','", *i);

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
                            fprintf(stderr, "error:%d: expected value, address or alpha", *i);

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
                            fprintf(stderr, "error:%d: expected character ')'", *i);

                            error = 1;
                            return 0;
                        }
                    }
                    *i += 1;

                    mem[addr] = val;

                    return 0;
                }

                case 0x01: /* store */
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
                            fprintf(stderr, "error:%d: expected value, address or alpha", *i);

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
                            fprintf(stderr, "error:%d: expected character ')'", *i);

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
                            fprintf(stderr, "error:%d: expected value, address or alpha", *i);

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
                                    fprintf(stderr, "error:%d: expected character ')'", *i);

                                    error = 1;
                                    return 0;
                                }
                            }
                        }

                        printf("%d", val);
                    }

                    return 0;
                }
            }
        }
        else
        {
            fprintf(stderr, "error:%d: expected character '('", *i);

            error = 1;
            return 0;
        }
    }

    return 0; 
}

int main()
{
    memset(mem, 0, 255);

    char input[255];

    puts("----------------------------------\n"
         "| Alpha Calculus                 |\n"
         "| Alpha has to be written as 'a' |\n"
         "----------------------------------");

    for(;;)
    {
        error = 0;

        printf("\na> ");
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
