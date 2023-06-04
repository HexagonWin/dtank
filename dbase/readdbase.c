#include <stdio.h>
#include <stdlib.h>

void main(int argc,char *argv[])
{
	struct dbf_form {
		char	id;	/* id x03:without .DBT file
				   x83:with    .DBT file */
		unsigned char	update_year;	/* Last updated Date */
		unsigned char 	update_month;
		unsigned char	update_date;
		unsigned long	rec_no;		/* 4 byte */
		unsigned int	header_len;	/* len of header structure */
		unsigned int	rec_len;	/* len of record */
		char	resv_1[20];		/* reserved bytes (Ver 1.00) */
		/* field descripter  32 bytes (fld_des) x field no 	*/
		/*		  .			*/
		/*		  .			*/
		/*		  .			*/
		/*		  .			*/
		/* terminator 0xd */

	} *dbf_header;

	struct	fld_des {
		char	fld_name[11];	/* Field name in ASCII zero filled */
		char	fld_type;	/* field type CNLDM */
		unsigned long fld_adrs;/* 4 byte */
		unsigned char fld_len;/* field len or len of Numeric len */
		unsigned char fld_dec;/* dec point len if Data type is Numeric */
		char	resv_2[14];	/* reserved bytes (Ver 1.00) */
	} *dbf_fld;


	FILE	*handle;
	int a,fi,fld_no,i;
	char *field_type_name[]={"Character","Numeric","Date","Logic","Memo"};
	char c;
	printf("\033[2J");
	if (argc<2) {
		printf("\n\n Usage : DBHDREAD TEST.DBF \n\n");
		exit(1);
	}
	dbf_header=(struct dbf_from *)malloc(sizeof(struct dbf_form));
	dbf_fld=(struct fld_des *)malloc(sizeof(struct fld_des));

	if ( (handle=fopen(argv[1],"rb")) == NULL ){
		printf("\n\nFile open error !!\n");
		exit(-1);
	}
	a=fread(dbf_header,sizeof(*dbf_header),1,handle);/* Read header */
	if (a != 1) {
		printf("\n File read error !!\n");
		exit(-1);
	}
	printf("id            = %d\n",dbf_header->id);
	printf("update(YY/MM/DD)=%02d/%02d/%02d\n",dbf_header->update_year,
					dbf_header->update_month,dbf_header->update_date);
	printf("number of rec = %ld \n",dbf_header->rec_no);
	printf("len of header = %d\n",dbf_header->header_len);

	fld_no = (dbf_header->header_len - 1 - 32) / 32;
			/*  field no = header len - terminator - pre header / field descript
er len */
	printf("Field no      = %d\n",fld_no);
	printf("len of rec    = %d\n",dbf_header->rec_len);

	for (i=0; i<fld_no; i++) {
	    /* Read Field descripter */
	    a=fread(dbf_fld,sizeof(*dbf_fld),1,handle);	
	    if (a != 1) {
		printf("\n File read error !!\n");
		exit(-1);
	    }
	    printf("\n\nField name  %s ",dbf_fld->fld_name);
	    switch(dbf_fld->fld_type)
	    {
		case 'C':	fi=0;	break;
		case 'N':	fi=1;	break;
		case 'D':	fi=2;	break;
		case 'L':	fi=3;	break;
		case 'M':	fi=4;	break;
		default:	fi=0;
	    }
	    printf(", %s\n",field_type_name[fi]);
	    printf("Field adrs = %ld \n",dbf_fld->fld_adrs);
	    printf("Field len  = %d",dbf_fld->fld_len);
	    if (fi==1)	printf(".%d",dbf_fld->fld_dec); /* if numeric */
	    getch();
	}
	a=fread(&c,1,1,handle);	/* Read Terminator 0xd */
	if (a != 1)
	{
		printf("\n File read error !!\n");
		exit(-1);
	}
	printf("\n\nterminator = %x\n",c);
	fclose(handle);
}

