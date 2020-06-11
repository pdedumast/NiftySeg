/**
 * @file seg_GetNCCscores.cpp
 * @authors M. Jorge Cardoso, P. de Dumast
 * @date 01/01/2014, 10/03/2020
 *
 * ADAPTED FROM:
 * seg_CalcTopNCC.cpp
 * author M. Jorge Cardoso
 * date 01/01/2014
 *
 * Copyright (c) 2014, University College London. All rights reserved.
 * Centre for Medical Image Computing (CMIC)
 * See the LICENSE.txt file in the nifty_seg root folder
 *
 *
 */

#include "_seg_tools.h"
#include "_seg_common.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;
#define SegPrecisionTYPE float


void Usage(char *exec)
{
    printf("\nUsage:\t%s <target_image> <template_image>\n\n",exec); //  <OPTIONS>\n\n",exec);
    // printf("\t* * Options * *\n");
    // printf("\t-mask <filename>\tFilename of the ROI mask\n");

#ifdef _GIT_HASH
    printf("\t--version\t\tPrint current source code git hash key and exit\n\t\t\t\t(%s)\n",_GIT_HASH);
#endif
    return;

}

int main(int argc, char **argv)
{


    char * filename_target=NULL;
    // char * filename_mask=NULL;

    //int numb_templates=1;
    //char ** filename_templates=NULL;
    char * filename_template=NULL;

    if(argc==1)
    {
        Usage(argv[0]);
        return 1;
    }

    if(argc != 3)
    {
        std::cout << "Too few/many arguments passed." << std::endl;
        return 1;
    }

    filename_target = argv[1];
    filename_template = argv[2];




    nifti_image * Image_Target=nifti_image_read(filename_target,true);
    if(filename_target==NULL)
    {
        fprintf(stderr, "This image can not be read: %s\n", filename_target);
        return 0;
    }
    seg_changeDatatype<float>(Image_Target);

    nifti_image * Mask=NULL;
    // if(filename_mask!=NULL)
    // {
    //     Mask = nifti_image_read(filename_mask,true);
    //     if(Mask->datatype!=DT_BINARY)
    //     {
    //         seg_convert2binary(Mask,0.5f);
    //     }
    //
    //     if(Mask == NULL)
    //     {
    //         fprintf(stderr,"* Error when reading the mask image: %s\n",filename_mask);
    //         return 1;
    //     }
    // }



    nifti_image * Image_template=NULL;
    float nccvalues;

    Image_template=nifti_image_read(filename_template,true);
    if(Image_template==NULL)
    {
        fprintf(stderr, "This image can not be read: %s\n", filename_template);
        return 0;
    }

    seg_changeDatatype<float>(Image_template);
    if(Image_Target->nx != Image_template->nx ||
            Image_Target->ny != Image_template->ny ||
            Image_Target->nz != Image_template->nz)
    {
        fprintf(stderr, "This image does not have the correct size: %s\n", filename_template);
        return 0;
    }

    nccvalues=estimateNCC3D(Image_Target,Image_template,Mask,0);

    nifti_image_free(Image_template);
    Image_template=NULL;


    cout<< nccvalues<<" " << endl;


    cout << endl;
    flush(cout);
    nifti_image_free(Image_Target);
    return 0;
}
