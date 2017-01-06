/*
* remod:    geoip.cpp
* date:     2007/2016
* author:   BudSpencer/degrave/Terence
*
* GEOIP new staff 
* Contains functions to get ip's country, city, region and timezone
*/

#ifdef GEOIP

#include "geoipmod.h"
#include "GeoIP.h"
#include "GeoIPCity.h"
#include "remod.h"
#include"remodex.h"

EXTENSION(GEOIP);

namespace remod
{
namespace geoip {
    static GeoIP *geoip = NULL;
    static GeoIP *geocity = NULL;
    
GeoIPtool *GIt = new GeoIPtool();
GeoIPLookup *gil = new GeoIPLookup();
GeoIP *gi = new GeoIP();

void loadgeoipcountry(char *dbname)
{
    const char *fname = findfile(dbname, "r"); // full path

    if(GIt->loaddb(fname))
    {
        conoutf(CON_ERROR, "Geoip: geocountry loaded (db: \"%s\")", fname);
    }
    else
    {
        conoutf(CON_ERROR, "Geoip: geocountry can not load (db: \"%s\")", fname);
    }
}


void loadgeoipcity(const char *path, bool isgeocity)
{
        GeoIP *gi;
        const char *fname = findfile(path, "r"); // full path
        gi = GeoIP_open(fname, GEOIP_STANDARD | GEOIP_MEMORY_CACHE);
        GeoIP_set_charset(gi, GEOIP_CHARSET_UTF8); //utf8 names for utf8 -> cubescript
        
        if(gi)
        {
            if(isgeocity)
                geocity = gi;
            else
                geoip = gi;
            conoutf(CON_ERROR, "Geoip: geocity loaded (db: \"%s\")", fname);
        }
        else
        {
            conoutf(CON_ERROR, "Geoip: geocity can ot load (db: \"%s\")", fname);
        }
}
    
void getcountry(char *ip)
{
    const char *country = NULL;
    country = GIt->getcountry(ip);
    if(!country) country = "Unknown Country";
    result(country);
}

const char *getcity( const char *addr )
{
    const char *city_name = NULL;

        GeoIPRecord *gir = GeoIP_record_by_addr( geocity, addr );
        if( gir != NULL ) 
        {
            char decoded_city[ MAXSTRLEN ];
            if ( gir->city ) 
            {
                decodeutf8( ( uchar* )decoded_city, MAXSTRLEN, ( uchar* )( gir->city ), MAXSTRLEN );
                city_name = newstring( decoded_city );
            }
    }

    return city_name;
}

const char *getregioncode(const char *addr)
{
    const char *region = NULL;
    GeoIPRecord *gir = GeoIP_record_by_addr(geocity, addr);
    if( gir != NULL )
    {
        char decoded_reg[ MAXSTRLEN ];
        if( gir->region )
        {
            decodeutf8( ( uchar* )decoded_reg, MAXSTRLEN, ( uchar* ) ( gir->region ), MAXSTRLEN );
            region = newstring( decoded_reg );
        }
    }
    return region;
}

void getregion(const char *address)
{
    const char *region = NULL;
    region = GeoIP_region_name_by_code(GIt->getcountrycode(address), getregioncode(address));
    if(!region) region = "Unknown Region";
    result(region);
}

void gettimezone(const char *adress)
{
    const char *tz = NULL;
    tz = GeoIP_time_zone_by_country_and_region(GIt->getcountrycode(adress), getregioncode(adress)); 
    if(!tz) tz = "Unknown Timezone";
    result(tz);
}

/**
 * Return country for specified IP-Adress
 * @group server
 * @arg1 ip
 * @return country
 */
COMMAND(getcountry,"s");

/**
 * Return city for specfied IP-Adress
 * @group server
 * @arg1 ip
 * @return city
 */
ICOMMAND(getcity, "s", (const char *addr),
{
    const char *city = getcity(addr);
    result(city != NULL ? city : "Unknown City");
});

/**
 * Return region for specfied IP-Adress
 * @group server
 * @arg1 ip
 * @return region
 */
COMMAND(getregion, "s");

/**
 * Return timezone for specified IP-Adress
 * @group server
 * @arg1 ip
 * @return timezone
 */
COMMAND(gettimezone, "s");

/**
 * Load geoipcity database from specified path
 * @group server
 * @arg1 /path/to/geoipcity.db
 */
ICOMMAND(geocitydb, "s", (const char *path),
{
    loadgeoipcity(path, true);
});

/**
 * Load geoipcountry database from specified path
 * @group server
 * @arg1 /path/to/geoipcountry.db
 */
COMMANDN(geocountrydb, loadgeoipcountry, "s");


/**
 * Check if geoip is ready to use
 * @group server
 * @return 1 if is ready, otherwise 0
 */
ICOMMAND(isgeoip, "", (), intret(GIt->loaded()));

}
}
#endif
