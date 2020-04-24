typedef const unsigned char *ICON_T;
#pragma pack(push, 1)
typedef struct
{
  short NIcons;         
  ICON_T const   *pIcons;
} ICON_PARAMS_T;
#pragma pack(pop)
extern const ICON_T  Icons[];
