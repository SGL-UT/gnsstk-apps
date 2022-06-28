# Winders powers hellllll
if ( !$env:CI_API_TOKEN )
{
    echo "Unable to locate API Token variable CI_API_TOKEN"
    exit 1
}
$WINDOWS_JOB="user_install_windows"
if ( $args[0] )
{
    $WINDOWS_JOB=$args[0]
}
$PKG_TYPE="pkg_min"
if ( $args[1] )
{
    $PKG_TYPE=$args[1]
}
$user=$env:CI_USER
$token=$env:CI_TOKEN
$repohost=$env:REPO_HOST
$giturl="https://"+$user+":"+$token+"@"+$repohost+"/sgl-tks/tks_ci_tools.git"
git clone --depth 1 $giturl
.\tks_ci_tools\tks_ci_tools\scripts\download_package_by_type.ps1 $WINDOWS_JOB $PKG_TYPE
exit
