wrapped_scripts = [
  ['makepkg.sh.in', '构建包'],
  ['rypm-db-upgrade.sh.in', '数据库升级'],
  ['rypm-key.sh.in', '密钥管理'],
  ['repo-add.sh.in', '仓库添加'],
]

scripts = [
  ['makepkg-template.pl.in', '包模板'],
]

SCRIPT_EDITOR = find_program(configure_file(
  input : join_paths(meson.project_source_root(), 'build-aux/edit-script.sh.in'),
  output : 'edit-script.sh',
  configuration : substs))

foreach item : scripts
  custom_target(
    item[0],
    input : item[0],
    command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@', '0755'],
    output : item[1],
    install : true,
    install_dir : get_option('bindir'))
endforeach

script_wrappers = []
foreach item : wrapped_scripts
  internal_script = custom_target(
    item[0],
    input : item[0],
    command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@', '0755'],
    output : item[1],
    install : true,
    install_dir : BINDIR)

  script_wrappers += [[ item[1], internal_script, meson.current_build_dir() ]]

  if item[0] == 'repo-add.sh.in'
    repo_add = internal_script
  endif
endforeach

foreach symlink : ['仓库删除', '仓库大象']
  meson.add_install_script(MESON_MAKE_SYMLINK,
                           '仓库添加',
                           join_paths(BINDIR, symlink))

  internal_script = custom_target(
    symlink,
    build_by_default : true,
    command : ['ln', '-sf', '仓库添加', '@OUTPUT@'],
    depends : repo_add,
    output : symlink)

  script_wrappers += [[ symlink, internal_script, meson.current_build_dir() ]]
endforeach

subdir('libmakepkg')

configure_file(
	configuration : { 'libmakepkgdir': LIBMAKEPKGDIR, 'PACKAGE_VERSION': PACKAGE_VERSION },
	input : 'libmakepkg.pc.in',
	output : '@BASENAME@',
	install_dir : join_paths(DATAROOTDIR, 'pkgconfig'))

custom_target(
  'bash_completion',
  command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@' ],
  input : 'completion/bash_completion.in',
  output : '包管理',
  install : true,
  install_dir : BASHCOMPDIR)

foreach symlink : ['包管理配置', '密钥管理', '构建包', '包模板', '仓库添加', '仓库删除', '版本比较', '包校验', '数据库升级']
  meson.add_install_script(MESON_MAKE_SYMLINK,
                           '包管理',
                           join_paths(BASHCOMPDIR, symlink))
endforeach

zsh_completion_dir = join_paths(DATAROOTDIR, 'zsh/site-functions')
custom_target(
  'zsh_completion',
  command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@' ],
  input : 'completion/zsh_completion.in',
  output : '_包管理',
  install : true,
  install_dir : zsh_completion_dir)
