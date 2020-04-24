#include "unicode.h"
#include "IconsDef.h"
#include "IconsRes.h"

const unsigned char UNICODE_ID[16] = {'Y','O','U',' ','M','U','S','T',' ','C','R','E','A','T','E',' '}; 

const ICON_PARAMS_T Icon_params_c = {BM_Ref_MAXVALUE, Icons};

const ICON_T Icons[BM_Ref_MAXVALUE] =
{
#include "IconsList.h"
};
