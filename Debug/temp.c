
/*VBE��Ϣ���ݽṹ����(VBE���ܵ���00H ����)*/
struct{																/*�ܹ�512�ֽ�*/
	char VBESignature[4];								/*�ַ���"VESA"*/
	unsigned int VBEVersion;						/*VBE�汾��,BCD��*/
	char far *OEMStringPtr;							/*ָ��OEM���̵ı�־����ָ��*/
	unsigned long Capabilities;					/*��ʾ������*/
	unsigned int far *VideoModePtr;			/*ָ������ʾģʽ�б��ָ��*/
	unsigned int VRAMMemory;						/*��ʾ�ڴ��С,��λδ64KB*/
	
	/*����ΪVESA VBE2.0�����϶���*/
	unsigned int OemSoftwareRev;				/*VBE�����OEM�޶��汾��,BCD��*/
	char far *OemVendorNamePtr;					/*ָ����ʾ�����쳧�̵��ַ���ָ��*/
	char far *OemProductNamePtr;				/*ָ����ʾ����Ʒ���Ƶ��ַ���ָ��*/
	char far *OemProductRevPtr;				  /*ָ����ʾ���޶��汾�Ż��Ʒ�ȼ����ַ���ָ��*/
	char reserved[222];									/*����*/
	
	char OEMData[256];									/*VESA2.0���ϰ汾����*/
}VBEInfo;

/*VBE�ض�ģʽ��Ϣ�����ݽṹ����(VBE���ܵ���01H����)*/
struct																			/*�ܹ�256�ֽ�*/
{
	unsigned int ModeAttr; 										/* ģʽ������ */
	unsigned char WinAAttr;				 						/* ����A������ */
	unsigned char WinBAttr;										/* ����B������ */
	unsigned short WinGran; 									/* ��������,��KBΪ��λ */
	unsigned short WinSize; 									/* ���ڴ�С,��KBΪ��λ */
	unsigned short WinASeg; 									/* ����A��ʼ��ַ */
	unsigned short WinBSeg; 									/* ����B��ʼ��ַ */
	char far *BankFunc;												/* ��ҳ������� */
	unsigned int BytesPerScanline; 						/* ÿ��ˮƽɨ������ռ���ֽ��� */
	unsigned int XRes,YRes; 								  /* Horizontal resolution */
	unsigned char XCharSize; 									/* �ַ��Ŀ�� */
	unsigned char YCharSize; 									/* �ַ��ĸ߶� */
	unsigned char NumberOfPlanes; 						/* λƽ��ĸ��� */
	unsigned char BitsPerPixel; 							/* ÿ���ص�λ�� */
	unsigned char NumberOfBanks;	 						/* CGA�߼�ɨ���߷����� */
	unsigned char MemoryModel; 								/* ��ʾ�ڴ�ģʽ */
	unsigned char BankSize; 									/* CGAÿ��ɨ���ߵĴ�С */
	unsigned char NumberOfImagePages; 				/* ��ͬʱ������������ͼ���� */
	unsigned char reserve1; 									/* Reserved */
	
	/*��ֱ��д��ɫģʽ�Ķ�������*/
	unsigned char RedMaskSize; 								/* ��ɫ��ռλ�� */
	unsigned char RedFieldPosition; 					/* ��ɫ�������Чλλ�� */
	unsigned char GreenMaskSize; 							/* ��ɫ��ռλ�� */
	unsigned char GreenFieldPosition; 				/* ��ɫ�������Чλλ�� */
	unsigned char BlueMaskSize; 							/* ��ɫ��ռλ�� */
	unsigned char BlueFieldPosition; 					/* ��ɫ�������Чλλ�� */
	unsigned char RsvdMaskSize; 							/* ����ɫ��ռλ�� */
	unsigned char RsvdFieldPosition; 					/* ����ɫ�������Чλλ�� */
	unsigned char DirectColorModeInfo; 				/* ֱ����ɫģʽ���� */
	
	/*����λVBE2.0�����϶���*/
	char far *PhyBasePtr;											/* ��ʹ�ô��֡����ʱΪָ������ַ��32λ�����ַ */
	unsigned long OffScreenMemOffset;					/* ֡������ַ��32λƫ����*/
	unsigned int OffScreenMemSize;						/* ���õġ���������ʾ������,��KBΪ��λ*/
	/*����ΪVBE3.0�����϶���*/
	unsigned int LinBytePerScanLine;					/* ���Ի�������ÿ��ɨ���ߵĳ��ȣ����ֽ�Ϊ��λ*/
	char BnkNumberOfImagePages;								/* ʹ�ô��ڹ���ʱ����ʾҳ����*/
	char LinNumberOfImagePages;								/* ʹ�ô�����Ի�����ʱ��ʾҳ����*/
	char LinRedMaskSize;											/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinRedFieldPosition;									/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinGreenMaskSize;										/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinGreenFieldPosition;								/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinBlueMaskSize;											/* ʹ�ô�����Ի�����ʱ��ɫ��ռλ��*/
	char LinBlueFieldPosition;								/* ʹ�ô�����Ի�����ʱ��ɫ�������Чλλ��*/
	char LinRsvMaskSize;											/* ʹ�ô�����Ի�����ʱ����ɫ��ռλ��*/
	char LinRsvFieldPosition;									/* ʹ�ô�����Ի�����ʱ����ɫ�������Чλλ��*/
	unsigned char reserve2[194]; 							/* ���� */
} ModeInfoBlock;