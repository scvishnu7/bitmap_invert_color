/*
* What this program does?
* -> Opens the Bitmap image file using open system call
* -> build a structure whihc contents standard headerfields of bitmap image.
* -> perform structure packing to avoid compiler expand the structure to integral multiple of sizeof int
* -> read the packed structure 
* -> get horezintal reso
* -> get vertical resolution
* -> loop  hor_reso*vert_res and read BGRA value 
* -> perform selected operation such as:invert the color 
* -> this way you get required output. 
*/
struct bmp_head {
	short sig;		//signature must be 0x4D42
	long size;		//size of bmp file unreliable
	short res1;		//reserved, must be zero
	short res2;		//reserved, must be zero
	long off_img_strt;	//offset start of image data in byte
	long siz_bmp_info_header;//size of BITMAPINFOHEADER structure, must be 40
	long img_width;		//image width in pixels
	long img_height;	//image height in pixels
	short no_plan;		//numbers of planes in image, must be 1
	short no_bit;		//number of bits per pixel 1,4,8,24
	long compr_type;	//compression type 0=none

	long size_img_data;	//size of image data in bytes incl padding
	long hor_res_met;	///horizontal resolution in pixels per meter unreliable
	long ver_res_met;	//vertical resolution in pixels per meter unreliable
	long no_col;		//number of colors in image, or zero
	long no_imp_col;	//number of important colors, or zero
}__attribute__((packed));
//this is requird to prevent compiler to padd the data with integer multiple of hte size of long//int used


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
int main(int argc,char **argv)
{
	int bmp_file,bmp_out;
	int read_size=0;
	struct bmp_head bmp_header;
	if(argc  != 3){
		printf("Usage: %s <inp_bmp_file> <out_bmp_file>\n",argv[0]);
		exit(0); }

	/* open input file */
	bmp_file = open(argv[1],O_RDONLY); // open in read only mode
	if(bmp_file <0) { printf("Error Opening file\n");exit(0);}
	printf("bmp opened with fd : %d\n",bmp_file);
	
	/* open the file for output*/
	bmp_out = open(argv[2],O_WRONLY|O_CREAT,S_IRWXO|S_IRWXU|S_IRWXG);//open in write mode
	if(bmp_out <0) { printf("Error Opening file for writing \n"); exit(0); }
	printf("bmp opend for writing successed\n");

	/* read the bitmap Header 1st */
	read_size = read(bmp_file,&bmp_header,sizeof(bmp_header));
	if(read_size==0){ printf("Error reading header info\n"); exit(0); }
	printf("bmp header read successfully\n");
	
	/* printing some header information */
	printf("%20s : %X\n","Signature",bmp_header.sig);
	printf("%20s : %ld\n","Unreliable Size",bmp_header.size);
	printf("%20s : %d\n","Reserved1",bmp_header.res1);
	printf("%20s : %d\n","Reserved2",bmp_header.res2);
	printf("%20s : %ld\n","Offset Image Start",bmp_header.off_img_strt);
	printf("%20s : %ld\n","size of BITMAPINFOHEADER",bmp_header.siz_bmp_info_header);
	printf("%20s : %ld\n","Image Width(pixels)",bmp_header.img_width);
	printf("%20s : %ld\n","Image Height(pixels)",bmp_header.img_height);
	printf("%20s : %d\n","No of Planes in Image",bmp_header.no_plan);
	printf("%20s : %d\n","No of bits per pixels",bmp_header.no_bit);
	printf("%20s : %ld\n","Compression Type",bmp_header.compr_type);
/*  Lets Start Reading from Left Bottom Corner with BGR Order. */

	read_size = write(bmp_out,&bmp_header,sizeof(bmp_header));

	char b,g,r,a;
	int operation=0;
	int i,j;
	int hor_res,vet_res;
	hor_res = bmp_header.img_width;
	vet_res = bmp_header.img_height;

	for(i=0;i<vet_res;i++){
		for(j=0;j<hor_res;j++){
		b=0;
		read(bmp_file,&b,1); read(bmp_file,&g,1); read(bmp_file,&r,1); read(bmp_file,&a,1);
		b = ~b;
		g = ~g;
		r = ~r;
		a = ~a;
		write(bmp_out,&b,1); write(bmp_out,&g,1); write(bmp_out,&r,1); write(bmp_out,&a,1);
//		printf(" <%X,%X,%X>  ",b,g,r);
	}
	}
	close(bmp_out);
	close(bmp_file);
return 0;
}
