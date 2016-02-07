#python.exe -c "import nose; nose.main()"
#might need a pip install nose --update to refind paths
#nosetests --with-cov --cov .
from mutester.nose_main import main

if __name__ == '__main__':
    main()
