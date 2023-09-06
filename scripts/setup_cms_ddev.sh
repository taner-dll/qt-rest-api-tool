#!/usr/bin/zsh



# chmod +x old_cms.sh
#add alias into .bashrc or .zshrc -> alias oldcms='/home/tnrdll/Shell\ Scripts/my_shell_scripts/old_cms.sh'


LGR='\033[1;32m' # LIGHT GREEN
NOC='\033[0m'    # NO COLOR
PUR='\033[0;35m' # PURPLE
RED='\033[0;31m' # RED



#t: tar file path
#s: sql file path
#command: ./cms_local_setup.sh -n pia -t cms.tar.gz -z file.sql.gz
while getopts n:t:z: flag
do
    case "${flag}" in
        n) dir_name=${OPTARG};;
        t) tar_file=${OPTARG};;
        z) database_file=${OPTARG};;
        *) echo "Invalid option: -$flag" ;;
    esac
done



#tar dosyasını aramak için indirilenler dizinine gözat
TAR_PATH="/home/tnrdll/archive/downloads/${tar_file}"
SQL_PATH="/home/tnrdll/archive/downloads/${database_file}"




if [[ -f ${TAR_PATH}  ]]
then
    printf "${LGR}${tar_file} bulundu.${NOC}\n"

else
    printf "${RED}Belirtilen dosya bulunamadı.${NOC}\n"
    exit 1
fi

if [[ -f ${SQL_PATH}  ]]
then
    printf "${LGR}${database_file} bulundu.${NOC}\n"

else
    printf "${RED}Belirtilen dosya bulunamadı.${NOC}\n"
    exit 1
fi



echo "islemler baslatiliyor..."
sleep 2
#read -p "Press enter to continue"

cp $TAR_PATH .
cp $SQL_PATH .


tar xvf $tar_file
gzip -d $database_file


rm -rf $tar_file
rm -rf $database_file

chmod -R 777 web/sites/default/


printf "${LGR}dosyalar başarıyla yerleştirildi.${NOC}\n"



drush cr
