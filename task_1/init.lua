local fio = require('fio')

local file = fio.open('config.yml')
local raw_conf = file:read()

file:close()

local yaml = require('yaml')
conf = yaml.decode(raw_conf)


local function hello(req)
	local req_st = http_client:request(req:method(), conf.proxy.bypass.host .. ':' .. conf.proxy.bypass.port)
	return
			{
				status = req_st.status,
				body = req_st.body
			}
end
--[[ 
{
	status = req_st.status,
	reason = req_st.reason,
	headers = req_st.headers,
	body = req_st.body,
	proto = req_st.proto
	}
]]

local router = require('http.router').new()
router:route({ method = 'GET', path = '/'}, hello)

local server = require('http.server').new('localhost', conf.proxy.bypass.port)
server:set_router(router)
server:start()

http_client = require('http.client').new({max_connections = 1})