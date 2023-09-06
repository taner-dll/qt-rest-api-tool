#!/usr/bin/zsh

# chmod +x new_cms.sh
#add alias into .bashrc or .zshrc -> alias newcms='/home/tnrdll/Shell\ Scripts/my_shell_scripts/new_cms.sh'

#n: gitlab name e.g: cms-pia
#d: database name e.g: pia_target

#command: newcms -n cms-pia -d pia_target
while getopts n:d: flag
do
    case "${flag}" in
        n) gitlab_name=${OPTARG};;
        d) db_name=${OPTARG};;
        *) echo "Invalid option: -$flag" ;;
    esac
done

cd "/var/www/html/oyas/";


git clone git@***/${gitlab_name}.git

cd $gitlab_name

composer install


#DRUPAL file permissions
cd "/var/www/html/$gitlab_name/web/sites/default"
cp default.settings.php settings.php
chmod a+w settings.php
echo "global \$content_directories;\$content_directories['sync'] = '../content/sync';" >> settings.php
echo "\$config['system.logging']['error_level'] = 'verbose';" >> settings.php



#DROP OLD DATABASE IF EXIST
if mysql "${db_name}" -e exit > /dev/null 2>&1; then
    mysql -e "DROP DATABASE ${db_name}";
fi



#INSTALL DRUPAL SITE via command
drush si minimal -y --db-url=mysql://root:***@localhost:3306/${db_name} --account-pass=*** --account-name=admin


chmod -R 777 files



#CONFIG-SET from old to new cms
#SOURCE_CMS_NAME="$(cut -d'-' -f2 <<< "${gitlab_name}")" #e.g (pia)
#SOURCE_CMS_UUID=$(drush cget --root="/var/www/html/oyas/"$SOURCE_CMS_NAME "system.site" uuid | cut -d ':' -f3 | xargs)
#echo $SOURCE_CMS_UUID
#drush cset "system.site" uuid $SOURCE_CMS_UUID -y



drush cr
cd "/var/www/html/oyas/$gitlab_name"

#READ MODULES variable form cfg file (needed for enabling modules)
#source "/var/www/html/oyas/${gitlab_name}/modules.cfg"
#echo $CUSTOM_MODULES

#ENABLE modules
#drush pm:enable -y $CONTRIB_MODULES
drush pm:enable --yes content_translation language
drush pm:enable --yes color_field adminimal_admin_toolbar admin_toolbar_links_access_filter admin_toolbar_tools devel features_ui




#SOURCE_CMS_LANG=$(drush cget --root="/var/www/html/"$SOURCE_CMS_NAME language.entity.en uuid | cut -d ':' -f3 | xargs)
#echo $SOURCE_CMS_LANG
#drush cset language.entity.en uuid $SOURCE_CMS_LANG -y



#CREATE SYNC DIRECTORY (needed for content sync module)
cd "/var/www/html/$gitlab_name"
mkdir -p content/sync
chmod a+w content/sync



#ENABLE THEME
drush theme:enable -y adminimal_theme
drush cset system.theme admin adminimal_theme -y
drush cset system.theme default adminimal_theme -y
chmod -R 777 "/var/www/html/$gitlab_name/web/sites/"

chown -R apache "/var/www/html/$gitlab_name/web/sites/"


echo "\$config['system.logging']['error_level'] = 'verbose';" >> web/sites/default/settings.php

#echo "\$settings['file_temp_path'] = '/tmp';" >> web/sites/default/settings.php

#COPY configs from old cms
#TARGET_CONFIG_PATH=$(find "/var/www/html/${gitlab_name}/web/sites/default/files/" -maxdepth 1 -type d -name '*config*' -print -quit)
#cp -r "/var/www/html/${SOURCE_CMS_NAME}/config/sync" $TARGET_CONFIG_PATH
#drush cim -y

drush cr
