#ifndef __BMP__H_
#define __BMP__H_


/*BMP�����ݽṹ 320*200 256ɫ*/
typedef struct BMP_file						/*�ļ���Ϣ��*/
{
	unsigned int bfType;						/*�ļ����� "BM"*/
	unsigned long bfSize;						/*�ļ�����*/
	unsigned int Reserved1;
	unsigned int Reserved2;
	unsigned long bfOffset;					/*�ļ����������ȣ�16ɫ118��256ɫ1024*/
}bitmapfile;	

typedef struct BMP_info						/*ͼ����Ϣ��*/
{
	unsigned long biSize;						/*ͼ�γߴ�*/
	unsigned long biWidth;					/*ͼ�ο��*/
	unsigned long biHeight;					/*ͼ�θ߶�*/
	unsigned int biPlanes;
	unsigned int biBitCount;				/*ÿ������ռ������λ��*/
	unsigned long biCompression;		/*�Ƿ�ѹ����ʽ*/
	unsigned long biSizeImage;
	unsigned long biXpolsPerMeter;
	unsigned long biYpelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
}bitmapinfo;

typedef struct RGB_BMP_typ				/*��ɫ����*//*256ɫ=256x4byte*/				
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
}RGB_BMP,*RGB_BMP_ptr;

typedef struct bmp_picture_typ
{
	bitmapfile file;
	bitmapinfo info;
	RGB_BMP palette[256];
	unsigned	char far *buffer;
} bmp_picture, *bmp_picture_ptr;

#endif