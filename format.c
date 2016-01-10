       #include <stdio.h>
       #include <stdarg.h>

void formatPercent(char proxy[64], char buff[64]){
	int i = 0, k = 0;
	while(proxy[i] == '\0'){
		if(proxy[i] == '%' && proxy[i + 1] == '%' ) {
			buff[k++] = '%';
			continue;
		}
		buff[k++] = proxy[i++];
	}	
}

       void
       foo(char *fmt, ...)
       {
           va_list ap;
           int d, i = 0, k = 0;
           char c, *s;
           char buff[128], proxy[128];

           va_start(ap, fmt);
           do{
			proxy[i++] = *fmt;
			if(*fmt++ == '%')
               switch (*fmt++) {
               case 's':              /* string */
					proxy[i++] = 's';
					proxy[i] = '\0';
                   s = va_arg(ap, char *);
                   sprintf(buff, proxy, s);
                   for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                   break;
               case 'd':              /* int */
					proxy[i++] = 'd';
					proxy[i] = '\0';
                   d = va_arg(ap, int);
                   sprintf(buff, proxy, d);
                   for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];

                   break;
               case 'c':              /* char */
                   /* need a cast here since va_arg only
                      takes fully promoted types */
					proxy[i++] = 'c';
					proxy[i] = '\0';
                   c = (char) va_arg(ap, int);
                   sprintf(buff, proxy, c);
                   for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                   break;
                case '%':
					//proxy[i++] = '%';
					proxy[i++] = *fmt++;
										
					break;
               }

             }while (*fmt);
           
           va_end(ap);

           printf("%s\n", proxy);
       }
       
   int main(){
		foo("select START, STOP from TASK where TIMEID = %d AND strftime('%%Y-%%m-%%d', STOP) = date('now');", 5);
		return 0;
	}
