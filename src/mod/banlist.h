/*
 * jsonbanlist.h
 *
 *  Created on: Jan 2, 2014
 *  Author: degrave/BudSpencer
 * 
 * askidbanlist, localbanlist
 */

#include "remod.h"

namespace remod
{
    namespace banlist
    {
        struct baninfo
        {
            union
            {
                enet_uint32 ip;
                uchar ipoctet[sizeof(enet_uint32)];
            };

            union
            {
                enet_uint32 mask;
                uchar maskoctet[sizeof(enet_uint32)];
            };
            time_t expire;
            time_t time;
            string admin;
            string reason;

            baninfo();
        };

        struct banlist
        {
            char *name;
            vector<baninfo*> bans;

            banlist();
            banlist(char *listname);

            void add(baninfo *ban);
            baninfo* remove(int n);
            size_t length();
        };
        
        struct askidbanlist 
        {
            char *name;
            vector<baninfo*> askidbanvec;
            askidbanlist();
            askidbanlist(char *listname);
            void add(baninfo *ban);
            baninfo* remove(int n);
            size_t length();
        };

        class banmanager
        {
            vector<banlist*> banlists;
            vector<askidbanlist*> askidbans;

            public:
            banmanager();
            banlist* getbanlist(char *name);
            askidbanlist* getaskidbanlist(char *name);
            banlist* localbanlist();
            askidbanlist* askidbanslist();
            baninfo* getban(char *listname, size_t n);
            baninfo* getaskidban(char *listname, size_t n);
            bool banlistexists(char *name);
            bool askidbanlistexists(char *name);
            void addban(char *listname, enet_uint32 ip, enet_uint32 mask, time_t expire, time_t time, const char *admin, const char *reason);
            void addaskidban(char *listname, enet_uint32 ip, enet_uint32 mask, time_t expire, time_t time, const char *admin, const char *reason);
            bool delban(char *listname, int id);
            bool checkban(enet_uint32 ip);
            bool checkaskidban(enet_uint32 ip);
            void parseipstring(char *ipstring, enet_uint32 &destip, enet_uint32 &destmask);
        };


    }
}
