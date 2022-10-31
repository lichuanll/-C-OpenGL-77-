#include "shader.h"

uint shader::compileShader(const char* _shaderPath, GLint _shaderType)
{
	if (_shaderPath == "" || _shaderPath == NULL)
	{
		return -1;
	}

	std::string     _shaderCode("");

	std::ifstream   _shaderFile;

	_shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		_shaderFile.open(_shaderPath);

		std::stringstream _shaderStream;
		_shaderStream << _shaderFile.rdbuf();

		_shaderCode = _shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::string errStr = "read shader fail";
		std::cout << errStr << std::endl;
	}
	const char* _shaderStr = _shaderCode.c_str();

	//shader�ı�������
	unsigned int   _shaderID = 0;
	char           _infoLog[512];
	int            _successFlag = 0;

	//����
	_shaderID = glCreateShader(_shaderType);
	glShaderSource(_shaderID, 1, &_shaderStr, NULL);
	glCompileShader(_shaderID);

	glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetShaderInfoLog(_shaderID, 512, NULL, _infoLog);
		std::string errStr(_infoLog);
		std::cout << _infoLog << std::endl;
	}

	return _shaderID;
}
void shader::initshader(const char* _vertexPath, const char* _fragPath)
{
	string _vertexCode("");
	string _fragCode("");

	ifstream _vShaderfile;//������ȡ�ļ�
	ifstream _fShaderfile;
	_vShaderfile.exceptions(ifstream::failbit | ifstream::badbit);//����try  catch
	_fShaderfile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		_vShaderfile.open(_vertexPath);//��ȡ�����ļ� 
		_fShaderfile.open(_fragPath);


		stringstream _vShaderStream, _fShaderStream;//����������
		_vShaderStream << _vShaderfile.rdbuf();//��ȡ�ļ�����
		_fShaderStream << _fShaderfile.rdbuf();

		_vertexCode = _vShaderStream.str();//��ȡ���벢����
		_fragCode = _fShaderStream.str();
	}

	catch (ifstream::failure e)
	{
		string errStr = "read shader fail";
		cout << errStr << endl;
	}

	const char* _vShaderStr = _vertexCode.c_str();
	const char* _fShaderStr = _fragCode.c_str();

	//shader�ı�������
	unsigned int _vertexID = 0, _fragID = 0;//����Ҫ�����ID 
	char _infoLog[512];//��¼�������ӹ����е�BUG
	int _successFlag = 0;

	//����
	_vertexID = glCreateShader(GL_VERTEX_SHADER);//����һ��shaderID
	glShaderSource(_vertexID, 1, &_vShaderStr, NULL);//���ߵ�ǰID��ʲô���Ĵ���
	glCompileShader(_vertexID);//��ʼ����������Ķ���

	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);//��׽������̻᲻�����Bug
	if (!_successFlag)//������벻�ɹ�
	{
		glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);//�õ���������ԭ��
		string errStr(_infoLog);//��ӡ������Ϣ
		cout << _infoLog << endl;
	}
	//����ͬ�϶���һ��ID����
	_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragID, 1, &_fShaderStr, NULL);
	glCompileShader(_fragID);

	glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}

	//����
	m_shaderProgram = glCreateProgram();//����һ��shader����
	glAttachShader(m_shaderProgram, _vertexID);//���������������õ�����shaer��ID   
	glAttachShader(m_shaderProgram, _fragID);
	glLinkProgram(m_shaderProgram);//�Ѽ���ĳ������ӳ�һ�������

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);//�ж��������л᲻�����
	if (!_successFlag)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);//�õ����ӹ����г���Ĵ�����Ϣ
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	glDeleteShader(_vertexID);//�����Ѿ����ɳ�������ɾ������ID
	glDeleteShader(_fragID);

}
void shader::intshader(const char* _vertexPath, const char* _fragPath, const char* _geoPath)
{
	//shader�ı���
	unsigned int   _vertexID = 0, _fragID = 0, _geoID = 0;
	_vertexID = compileShader(_vertexPath, GL_VERTEX_SHADER);
	_fragID = compileShader(_fragPath, GL_FRAGMENT_SHADER);
	_geoID = compileShader(_geoPath, GL_GEOMETRY_SHADER);

	char           _infoLog[512];
	int            _successFlag = 0;

	//����
	m_shaderProgram = glCreateProgram();
	if (_vertexID != -1)
	{
		glAttachShader(m_shaderProgram, _vertexID);
	}
	if (_fragID != -1)
	{
		glAttachShader(m_shaderProgram, _fragID);
	}
	if (_geoID != -1)
	{
		glAttachShader(m_shaderProgram, _geoID);
	}
	glLinkProgram(m_shaderProgram);

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
		std::string errStr(_infoLog);
		std::cout << _infoLog << std::endl;
	}
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
	glDeleteShader(_geoID);
}

void shader::setFloat(const string& _name, float _f)const
{
	glUniform1f(glGetUniformLocation(m_shaderProgram, _name.c_str()), _f);
}

void shader::setInt(const string& _name, int _i)const
{
	glUniform1i(glGetUniformLocation(m_shaderProgram, _name.c_str()), _i);
}

void shader::setMatrix(const string& _name, mat4 _matrix)const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, GL_FALSE, value_ptr(_matrix));

}
void shader::setvec3(const string& _name, vec3 _vec3)const
{
	glUniform3fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, value_ptr(_vec3));
}
void shader::setvec4(const string& _name, vec4 _vec4)const
{
	glUniform4fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, value_ptr(_vec4));

}