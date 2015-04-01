from distutils.core import setup
import py2exe

py2exe_options = dict (
    dll_excludes = ['MSVCR80.dll','msvcp80.dll'],
    excludes = ['pyreadline','doctest','locale',
                'pickle','calendar','doctest','pdb',
                'inspect','email','tcl','tk',
                'Tkconstants','Tkinter'],
)

setup(console=['poclbm.py'],options = {'py2exe':py2exe_options})
