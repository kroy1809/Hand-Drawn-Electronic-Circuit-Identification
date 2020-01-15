#include<stdio.h>
#include<stdlib.h> 
#include<math.h> 

int r=0,c=0,l=0,v=0,i=0,dc=0,gnd=0,diode=0; 

struct header
{
    char filetype;          
    long int file_size;     
    long reserved;          
    long data_offset,size_info,width,height;    
    int  no_planes, bpp;            
    long image_size;    
    long xresolution;   
    long yresolution;   
    int colourind;  
    int colourimp;      
    int value;   
}H; 

typedef struct 
{
    char magic1[2];
    char size[4];     
    char magic2[44]; 
}bmp_t; 

struct store 
{
    int a;  
    int b; 
}s1,s2; 

void main()
{
    char image[400]; 
    int x; 

    printf("Enter the name of the image file with extension:"); 
    scanf("%s",image);

    x=input(image); 
    x=output(); 

    printf("Generating spice code...\n"); 
    printf("Code has been generated!\n");
}

int input(char filename[])
{
    long filesize; 
    char *buffer; 

    FILE * file = fopen(filename, "rb"); 

    fread(&H.filetype,1,1,file );    
    printf("Type of file is %c", H.filetype);
    fseek(file,1,0);
    fread(&H.filetype,1,1,file);        
    
    printf("%c\n",H.filetype);
    fseek(file, 0, SEEK_END);   
    filesize = ftell(file);     
    
    rewind(file);   
    buffer = (char *) malloc(sizeof(char) * filesize);  
    fread(buffer, 1, filesize, file);   
    bmp_t * bmp = (bmp_t *) buffer;         
    printf("Size: %02x %02x %02x %02x bytes(hex) \n", bmp->size[0] & 0xFF, bmp->size[1] & 0xFF, bmp->size[2] & 0xFF, bmp->size[3] & 0xFF);
    fseek(file,6,0);
    fread(&H.reserved,2,1,file);        
    printf("Reserved %ld\n",H.reserved);    
    fseek(file,8,0);    
    fread(&H.reserved,2,1,file);        
    printf("Reserved %ld\n",H.reserved);    
    fseek(file,10,0);   
    fread(&H.data_offset,4,1,file);         
    printf("Data offset at %ld\n",H.data_offset);   
    fseek(file,14,0);   
    fread(&H.size_info,4,1,file);       
    printf("Size of file info header %ld\n",H.size_info);   
    fseek(file,18,0);   
    fread(&H.width,4,1,file);       
    printf("Width of image %ld pixels \n",H.width);     
    fseek(file,22,0);   
    fread(&H.height,4,1,file);      
    printf("Height of image %ld pixels \n",H.height);   
    fseek(file,50,0);   
    fread(&H.colourimp,4,1,file);   
    int i,j,k,r,g,b,x;  
    float gsc,gray[H.height][H.width],s=0.0;    
    int red[H.height][H.width],green[H.height][H.width],blue[H.height][H.width];    
    for(i=0;i<H.height;i++)     
    {       
        for(j=0;j<H.width;j++)      
        {           
            for(k=0;k<3;k++)            
            {               
                fread(&H.value,1,1,file);               
                switch(k)               
                {                   
                    case 0:                     
                    {                       
                        b=H.value; 
                        break;
                    }
                    case 1:                     
                    {                       
                        g=H.value;                      
                        break;                  
                    }                   
                    case 2:                     
                    {                       
                        r=H.value;                      
                        break;                  
                    }                   
                    default:                    
                    {                       
                        break;                  
                    }               
                }//switch           
            }//k            
            blue[i][j]=b;           
            green[i][j]=g;          
            red[i][j]=r;            
            gsc=(0.2126*r)+(0.7152*g)+(0.0722*b);           
            gray[i][j]=gsc;         
        }       
        x=H.width%4;        
        switch(x)       
        {           
            case 0:             
            {               
                break;          
            }           
            case 1:             
            {               
                fread(&H.value,1,1,file);               
                break;          
            }           
            case 2:             
            {               
                fread(&H.value,1,1,file);               
                fread(&H.value,1,1,file);
                break;          
            }           
            case 3:             
            {               
                fread(&H.value,1,1,file);               
                fread(&H.value,1,1,file);               
                fread(&H.value,1,1,file);               
                break;          
            }           
            default:            
            {               
                break;          
            }       
        }//switch   
    }//i 

    int arr1[H.height][H.width];    
    int flag1[H.height];    
    int flag2[H.height];    
    for (i=0;i<H.height;i++)    
    {       
        flag1[i]=0;         
        flag2[i]=0;     
    }   

    FILE *fp1;  
    fp1=fopen("Output.ascii","w");  
    
    for (i=0;i<H.height;i++)    
    {       
        for(j=0;j<H.width;j++)      
        {           
            if(gray[i][j]>=40)              
                arr1[i][j]=1;           
            else                
                arr1[i][j]=0;           

            if(i!=0 || i!=1)            
            {               
                if(arr1[i][j]!=arr1[i-1][j])                    
                    flag1[i]=1;                 
                if(arr1[i][j]!=arr1[i-2][j])                    
                    flag2[i]=1; 
            }                       
        }   
    }   
    
    int k1=0;   
    for (i=0;i<H.height;i++)    
    {       
        if(flag1[i]!=0 || flag2[i]!=1)      
        {           
            k1++;           
        }   
    }   

    int arr[k1][H.width];   
    int kr=0;   
    
    for (i=0;i<H.height;i++)    
    {       
        if(flag1[i]!=0 || flag2[i]!=1)      
        {           
            for (j=0;j<H.width;j++)                 
                arr[kr][j]=arr1[i][j];          
            kr++;       
        }   
    }   

    for (i=0;i<kr;i++)  
    {       
        for(j=0;j<H.width;j++)      
        {           
            fprintf(fp1,"%d",arr[i][j]);        
        }       
        fprintf(fp1,"\n");  
    }   
    fclose(fp1); 

    return 0;    
} 

//Read Array from a file 
output() 
{     
    FILE *fp;   
    fp = fopen("Output.ascii","r");     
    char ch;    
    int r=0,c=0;    
    if(fp == NULL)  
    {       
        perror("Error while opening the file.");        
        exit(EXIT_FAILURE);     
    }   

    ch=getc(fp);    

    while(ch!=EOF)  
    {       
        if(ch=='\n')            
            r++;        
        else if(ch!='\n')           
            c++;        

        ch=getc(fp);    
    }   

    c=c/r;  

    int x1=0,x2=0;  
    int arr[r][c];  
    fclose(fp);     
    fp = fopen("Output.ascii","r");     
    ch=getc(fp);    

    while(ch!=EOF)  
    {       
        if(ch=='\n')        
        {           
            x1++;           
            x2=0;       
        }       
        else
        {           
            arr[x1][x2]=ch-'0';             
            x2++;       
        }           
        ch=getc(fp);    
    }   
    fclose(fp);     

    int i,j,flag=0,cnt=0;   
    int b[r*c];         

    //Finding first '0'     
    for(i=0;i<r;i++)    
    {       
        for(j=0;j<c;j++)        
        {           
            if(arr[i][j]==0)            
            {               
                s1.a=i;                 
                s1.b=j;                 
                flag=1; 
            }           
            if(flag==1)                
                break;      
        }       
        if(flag==1)             
            break;  
    }   
      
    //Finding pattern     
    for(i=0;i<r;i++)    
    {       
        for(j=0;j<c;j++)        
        {           
            if(arr[i][j]==0)            
            {               
                cnt++;              
                if(cnt==1)                  
                    continue;               
                else                
                {                   
                    s2.a=i;                     
                    s2.b=j;                     
                    b[cnt-2]=relation(s1,s2);                   
                    s1=s2;              
                }           
            }       
        }   
    }   
    
    //Run Length Encoding   
    int out=0,k,k1,k2,l=cnt-1;  
    int diff_log[cnt];  
    k2=0;   
    for (i=0;i<(l-1);i++)   
    {       
        out=b[i+1]-b[i];        
        if (out==0)             
            diff_log[i]=0;      
        else        
        {           
            diff_log[i]=1;          
            k2++;       
        }   
    }   
    diff_log[l-1]=1;    
    k=0;    

    int pos[k2];    
    for (i=0;i<l;i++)   
    {       
        if (diff_log[i]==1)         
        {           
            pos[k]=1;           
            k++;        
        }   
    }   

    int ele[k-1]; 
    k1=0;   

    for(i=0;i<l;i++)    
    {       
        if (diff_log[i]==1)         
        {           
            ele[k1]=b[i];           
            k1++;       
        }   
    }       
    
    printf("Identifying elements...\n");       
    //Identifying Elements    
    int x;  
    cnt=0;  

    for(i=0;i<k1-1;i++)     
    {       
        if(ele[i]==5)       
        {
            cnt++; 
            x=element(ele,i,k1,cnt); 
            if(x==-1) 
                cnt--;
        } 
    }     
    printf("\n");       
    return 0;
} 

//Function to return relationship between two zeroes 
int relation(struct store s1,struct store s2) 
{    
    int a=s2.a-s1.a;    
    int b=s2.b-s1.b;    

    if(a==0 && b==-1)
       return 7;
    if(a==0 && b==1)
        return 3;   
    if(a==-1 && b==0)       
        return 1;   
    if(a==1 && b==0)        
        return 5;   
    if(a==1 && b==1)        
        return 4;   
    if(a==1 && b==-1)           
        return 6;   
    if(a==-1 && b==-1)      
        return 8;   
    if(a==-1 && b==1)
        return 2;   
    else        
        return 9; 
} 

//Function to detect the elements
int element(int arr[],int k,int size,int c) 
{   
    int i,sum=0,cnt=0;  
    double avg,sd,sum1=0;   

    for(i=k+1;i<size;i++)   
    {       
        if(arr[i]==5)           
            break;      
        else        
        {           
            sum=sum+arr[i];             
            cnt++;      
        }   
    }   

    avg=(float)((float)sum/(float)cnt);     
    for(i=(k+1);i<size;i++)     
    {       
        if(arr[i]==5)           
            break;      
        else        
        {           
            sum1=sum1+pow((arr[i]-avg),2);      
        }   
    }   
    sd=(float)((float)sum1/(float)cnt);     
    sd=sqrt(sd);    

    int sdi=(int)(sd*100);  
    printer(sdi,c);       

    if(sdi==48)           
        return -1;       
    if(sdi==284)           
        return -1;       
    return sdi; 
} 

printer(int n,int cnt)
{       
    int temp;       
    char t[400];       

    FILE *fp2;   
    fp2=fopen("Spice.cir","a");       
    switch(n) 
    { 
        case 100:         
            fprintf(fp2,"R");       
            r++;        
            fprintf(fp2,"%d",r);        
            fprintf(fp2," %d %d ",cnt-1,cnt);       
            printf("\nEnter value of resistor:");       
            scanf("%d",&temp);      
            printf("Enter units:");         
            scanf("%s",t);      
            fprintf(fp2,"%d",temp);         
            fprintf(fp2,"%s\n",t);
            break; 
        case 293:        
            fprintf(fp2,"C");       
            c++;        
            fprintf(fp2,"%d",c);        
            fprintf(fp2," %d %d ",cnt-1,cnt);       
            printf("\nEnter value of capacitor:");      
            scanf("%d",&temp);      
            printf("Enter units:");         
            scanf("%s",t);      
            fprintf(fp2,"%d",temp);         
            fprintf(fp2,"%s\n",t);      
            break; 
        case 299:        
            fprintf(fp2,"V");       
            v++;        
            fprintf(fp2,"%d",v);        
            fprintf(fp2," %d %d ",cnt-1,cnt);       
            printf("\nEnter value of voltage:");        
            scanf(" %d",&temp);         
            printf("Enter units:");         
            scanf("%s",t);      
            fprintf(fp2,"%d",temp);         
            fprintf(fp2,"%s\n",t);      
            break; 
        case 300:        
            fprintf(fp2,"V");       
            v++;        
            fprintf(fp2,"%d",v);        
            fprintf(fp2," %d %d ",cnt-1,0);         
            fprintf(fp2,"0V \n");       
            break; 
        case 224:    
            fprintf(fp2,"I"); 
            i++; 
            fprintf(fp2,"%d",i); 
            fprintf(fp2," %d %d ",cnt-1,cnt);  
            printf("\nEnter value of current:"); 
            scanf("%d",&temp); 
            printf("Enter units:"); 
            scanf("%s",t); fprintf(fp2,"%d",temp); 
            fprintf(fp2,"%s\n",t); 
            break;
        case 141:   
            fprintf(fp2,"V"); 
            v++; 
            fprintf(fp2,"%d",v); 
            fprintf(fp2," %d %d ",cnt-1,cnt);  
            printf("\nEnter value of voltage:"); 
            scanf("%d",&temp); 
            printf("Enter units:"); 
            scanf("%s",t); 
            fprintf(fp2,"AC %d",temp); 
            fprintf(fp2,"%s\n",t); 
            break;
        case 296:
            fprintf(fp2,"L");       
            l++;        
            fprintf(fp2,"%d",l);        
            fprintf(fp2," %d %d ",cnt-1,cnt);       
            printf("\nEnter value of inductor:");       
            scanf("%d",&temp);      
            printf("Enter units:");         
            scanf("%s",t);      
            fprintf(fp2,"%d",temp);         
            fprintf(fp2,"%s\n",t);      
            break;
    }
}