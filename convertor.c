#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlschemastypes.h>

int swap_Endians(int value)
{

    int leftmost_byte;
    int left_middle_byle;
    int right_middle_byte;
    int rightmost_byte;
    int result;

    leftmost_byte = (value & 0x000000FF) >> 0;
    left_middle_byle = (value & 0x0000FF00) >> 8;
    right_middle_byte = (value & 0x00FF0000) >> 16;
    rightmost_byte = (value & 0xFF000000) >> 24;
    leftmost_byte <<= 24;
    left_middle_byle <<= 16;
    right_middle_byte <<= 8;
    rightmost_byte <<= 0;
 
    // Result is the concatenation of all these values.
    result = (leftmost_byte | left_middle_byle |
              right_middle_byte | rightmost_byte);
 
    return result;
}
 
  

struct record {
    char name[64]; //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure; // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};
typedef struct record _record;

int main()
{
    
    _record wItem,rItem;
    FILE *fp;
    fp = fopen("records.dat", "rb");
    if(fp == NULL)
    {
        printf("Error opening file!");
        exit(1);
    }

    FILE *f;
    f = fopen("records.xml","w");
    if(f==NULL){
        printf("Error");
        return 0;
    
    }
    fseek(fp , 252 , SEEK_SET);
    fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf(f,"<records>\n");
    int row_id = 0;
    int i = 0;
    while(!feof(fp)){
        fread(&rItem, sizeof(_record) , 1 , fp);
        row_id = row_id +1;
        if(row_id == 51){
            break;
        }
        fprintf(f,"\t<row id = \"%d\">\n",row_id);
        fprintf(f,"\t\t<name> %s </name>\n",rItem.name);
        fprintf(f,"\t\t<surname> %s </surname>\n",rItem.surname);
        fprintf(f,"\t\t<gender> %c </gender>\n",rItem.gender);
        fprintf(f,"\t\t<email> %s </email>\n",rItem.email);
        fprintf(f,"\t\t<phone_number> %s </phone_number>\n",rItem.phone_number);
        fprintf(f,"\t\t<address> %s </address>\n",rItem.address);
        fprintf(f,"\t\t<level_of_education> %s </level_of_education>\n",rItem.level_of_education);
        fprintf(f,"\t\t<income_level> %u </income_level>\n",rItem.income_level);
        int expenditure= swap_Endians(rItem.expenditure);
        fprintf(f,"\t\t<expenditure> %u </expenditure>\n",expenditure);
        fprintf(f,"\t\t<currency_unit> %s </currency_unit>\n",rItem.currency_unit);
        fprintf(f,"\t\t<current_mood> %s </current_mood>\n",rItem.currentMood);
        fprintf(f,"\t\t<height> %f </height>\n",rItem.height);
        fprintf(f,"\t\t<weight> %u </weight>\n",rItem.weight);
        fprintf(f,"\t</row>\n");
    }
    fprintf(f,"</records>\n");
    fclose(f);       
    fclose(fp);

    
////-------------------------VALIDATION-------------------------/////////

    /*
    xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	char *XMLFileName = "records.xml";
	char *XSDFileName = "records.xsd";
	int ret;
 
	xmlLineNumbersDefault(1);
 
	ctxt = xmlSchemaNewParserCtxt(XSDFileName);
 
	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
 
 
	doc = xmlReadFile(XMLFileName, NULL, 0);
 
	if (doc == NULL){
		fprintf(stderr, "Could not parse %s\n", XMLFileName);
	}
	else{
		xmlSchemaValidCtxtPtr ctxt;
 
		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
 
		if (ret == 0){
			printf("%s validates\n", XMLFileName);
		}
		else if (ret > 0){
			printf("%s fails to validate\n", XMLFileName);
		}
		else{
			printf("%s validation generated an internal error\n", XMLFileName);
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}
 
 
	if(schema != NULL)
		xmlSchemaFree(schema);
 
	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();
 

    */
    return 0;
}