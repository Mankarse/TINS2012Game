#ifndef TINSGame2012_Filesystem_h
#define TINSGame2012_Filesystem_h
#include <allegro5/allegro.h>
struct FilesystemEntry {
    FilesystemEntry(ALLEGRO_FS_ENTRY *fsEntry=0): fsEntry(fsEntry){}
    
    ALLEGRO_FS_ENTRY* get() {
        return fsEntry;
    }
    
    ALLEGRO_FS_ENTRY* release() {
        ALLEGRO_FS_ENTRY *retV = fsEntry;
        fsEntry = 0;
        return retV;
    }
    
    void reset(ALLEGRO_FS_ENTRY* const fsEntry=0) {
        al_destroy_fs_entry(this->fsEntry);
        this->fsEntry = fsEntry;
    }
    
    ~FilesystemEntry(){
        al_destroy_fs_entry(fsEntry);
    }
private:
    FilesystemEntry(FilesystemEntry const&);
    FilesystemEntry& operator=(FilesystemEntry const&);
    ALLEGRO_FS_ENTRY *fsEntry;
};

struct DirectoryManager {
    DirectoryManager(ALLEGRO_FS_ENTRY *fsEntry) :fsEntry(fsEntry), open(al_open_directory(fsEntry)){}
    ~DirectoryManager() {
        if (open) {
            al_close_directory(fsEntry);
        }
    }
    bool is_open() const { return open; }
private:
    ALLEGRO_FS_ENTRY *fsEntry;
    bool open;
};
#endif
