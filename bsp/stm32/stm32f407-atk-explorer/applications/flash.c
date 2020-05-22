#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#include <dfs_posix.h>
#include <dfs_romfs.h>

void harddisk_mount()
{
    static const rt_uint8_t _romfs_root_readme_txt[] = { 
        "ROMFS Root: SDCARD and SPI FLASH are both mounted on it\n",
    };
    static const rt_uint8_t _romfs_root_sdcard_aaa_txt[] = {
        0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
    };

    static const struct romfs_dirent _romfs_root_sdcard[] = {
        {ROMFS_DIRENT_FILE, "aaa.txt", (rt_uint8_t *)_romfs_root_sdcard_aaa_txt, sizeof(_romfs_root_sdcard_aaa_txt)/sizeof(_romfs_root_sdcard_aaa_txt[0])}
    };

    static const rt_uint8_t _romfs_root_spi_bbb_txt[] = {
        0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
    };

    static const struct romfs_dirent _romfs_root_spi[] = {
        {ROMFS_DIRENT_FILE, "bbb.txt", (rt_uint8_t *)_romfs_root_spi_bbb_txt, sizeof(_romfs_root_spi_bbb_txt)/sizeof(_romfs_root_spi_bbb_txt[0])}
    };

    static const struct romfs_dirent _romfs_dirent[] = {
        /*
        {ROMFS_DIRENT_DIR, "sdcard", (rt_uint8_t *)_romfs_root_sdcard, sizeof(_romfs_root_sdcard)/sizeof(_romfs_root_sdcard[0])},
        {ROMFS_DIRENT_DIR, "flash", (rt_uint8_t *)_romfs_root_spi, sizeof(_romfs_root_spi)/sizeof(_romfs_root_spi[0])},
        */
        {ROMFS_DIRENT_DIR, "sdcard", 0,0},
        {ROMFS_DIRENT_DIR, "flash", 0,0},
				{ROMFS_DIRENT_DIR, "udisk", 0,0},
        {ROMFS_DIRENT_FILE, "readme.txt", (rt_uint8_t *)_romfs_root_readme_txt, sizeof(_romfs_root_readme_txt)/sizeof(_romfs_root_readme_txt[0])}
    };

    static const struct romfs_dirent romfs_root =
    {
        ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_dirent, sizeof(_romfs_dirent) / sizeof(_romfs_dirent[0])
    };

    if (dfs_mount(RT_NULL, "/", "rom", 0, &(romfs_root)) == 0)
    {
        rt_kprintf("ROM file system initializated!\n");
    }
    else{
        rt_kprintf("ROM file system Failed!\n");
    }

    #if 1
		
    rt_device_t dev = rt_device_find("W25Q128");
    if(dev!=RT_NULL){
        if (dfs_mount("W25Q128", "/flash", "elm", 0, 0) == 0)
        {
            rt_kprintf("spi flash mount to /spi !\n");
        }
        else
        {
            rt_kprintf("spi flash mount to /spi failed!\n");
        }
    }
        
    dev = rt_device_find("sd0");
    if(dev!=RT_NULL){
        if (dfs_mount("sd0", "/sdcard", "elm", 0, 0) == 0)
        {
            rt_kprintf("sd card mount to /sdcard!\n");        
        }
        else
        {
            rt_kprintf("sd card mount to /sdcard failed!\n");
        }
    } else {
        rt_kprintf("Can not find device: sd0\n");
			dev = rt_device_find("ud0-0");
    if(dev!=RT_NULL){
        if (dfs_mount("ud0-0", "/udisk", "elm", 0, 0) == 0)
        {
            rt_kprintf("udisk mount to /udisk!\n");        
        }
        else
        {
            rt_kprintf("udisk mount to /udisk failed!\n");
        }
    } else {
        rt_kprintf("Can not find device: ud0-0\n");
    }
    }
    #endif
}

INIT_ENV_EXPORT(harddisk_mount);
