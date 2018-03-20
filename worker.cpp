#include "worker.h"
#include <QDir>

Worker::Worker(QString p, QString n, QString o, QObject *parent) : path(p), name(n), obj(o), QObject(parent)
{
}

void Worker::run(){
    if(name!="")
        path.append("/").append(name);

    watchFolder(path);

    emit Worker::Finish();
}

void Worker::watchFolder(const QString& dir){
    QDir curDir(dir);

    if(curDir.exists()){
        QFileInfoList files = curDir.entryInfoList();

        for(QFileInfo file : files){
            if(file.isDir()&&file.absoluteFilePath()!=dir&&file.absolutePath()==dir)
                watchFolder(file.absoluteFilePath());
            else if(file.isFile()){
                QString absolutePath = file.absolutePath();
                QString fileName = absolutePath.mid(absolutePath.lastIndexOf('/')+1, absolutePath.size() - absolutePath.lastIndexOf('/')-1);
                absolutePath.append("/").append(fileName).append(".RAW");
                QFile f(absolutePath);

                if(!f.exists())
                    break;

                f.open(QFile::ReadOnly);
                if(f.isOpen()){
                    if(isGood(f)){
                        emit Worker::Find(file.absolutePath());
                    }
                    f.close();
                }
                else
                    emit Worker::Error("Can't open "+absolutePath);

                return;
            }
        }
    }else{
        emit Worker::Error("Folder doesn't exists");
        return;
    }
}

bool charsCompare(const char &c1, const char &c2){
    if(c1>='a'&&c1<='z'){
        if(c2>='A'&&c2<='Z')
            return (c1-'a'==c2-'A');

    }else if(c1>='A'&&c1<='Z'){
        if(c2>='a'&&c2<='z')
            return (c1-'A'==c2-'a');
    }

    return (c1==c2);
}

bool Worker::isGood(QFile & f){
    QByteArray bytes = f.readAll();
    int cur_byte = 76;
    while(cur_byte-76<obj.size() && bytes[cur_byte]!=0 && charsCompare(bytes[cur_byte],obj[cur_byte-76].toLatin1())){
        cur_byte++;
    }

    return (cur_byte-76==obj.size()&&bytes[cur_byte]==0);
}
