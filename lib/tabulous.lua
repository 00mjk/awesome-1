---------------------------------------------------------------------------
-- tabulous: fabulous tabs for awesome
--
-- @author Lucas de Vries <lucasdevries@gmail.com>
-- @author Julien Danjou <julien@danjou.info>
-- @copyright 2008 Julien Danjou, Lucas de Vries
---------------------------------------------------------------------------

-- Grab environment we need
local capi = { client = client }
local table = table
local pairs = pairs
local awful = require('awful')

module("tabulous")

local tabbed = {}

-- Hook creation
awful.hooks.user.create('tabbed')
awful.hooks.user.create('untabbed')
awful.hooks.user.create('tabdisplay')
awful.hooks.user.create('tabhide')

---------------
-- Functions --
---------------

--- Find a key in a table.
-- @param table The table to look into
-- @param value The value to find.
-- @return The key or nil if not found.
function findkey(table, value)
    for k, v in pairs(table) do
        if v == value then
            return k
        end
    end
end

--- Swaand select which client in tab is displayed.
-- @param tabindex The tab index.
-- @param cl The client to show.
function display(tabindex, cl)
    local p = tabbed[tabindex][1]
    if cl and p ~= cl then
        cl.hide = false
        cl:swap(p)
        p.hide = true
        cl:focus_set()

        tabbed[tabindex][1] = cl

        awful.hooks.user.call('tabhide', p)
        awful.hooks.user.call('tabdisplay', cl)
    end
end

--- Check if the client is in the given tabindex.
-- @param tabindex The tab index.
-- @param cl The client
-- @return The key.
function tabindex_check(tabindex, cl)
    local c = cl or capi.client.focus_get()
    return findkey(tabbed[tabindex], c)
end

--- Find the tab index or return nil if not tabbed.
-- @param cl The client to search.
-- @return The tab index.
function tabindex_get(cl)
    local c = cl or capi.client.focus_get()

    for tabindex, tabdisplay in pairs(tabbed) do
        -- Loop through all tab displays
        local me = tabindex_check(tabindex, c)

        if me ~= nil then
            return tabindex 
        end
    end

    return nil
end

--- Get all clients on a tabbed display.
-- @param tabindex The tab index.
-- @return All tabbed clients.
function clients_get(tabindex)
    if tabbed[tabindex] == nil then return nil end
    return tabbed[tabindex][2]
end

--- Get the displayed client on a tabbed display.
-- @param tabindex The tab index.
-- @return The displayed client.
function displayed_get(tabindex)
    if tabbed[tabindex] == nil then return nil end
    return tabbed[tabindex][1]
end

--- Get a client by tab number.
-- @param tabindex The tab index.
-- @param pos The position in the tab.
-- @return The client at the given position.
function position_get(tabindex, pos)
    if tabbed[tabindex] == nil then return nil end
    return tabbed[tabindex][2][pos]
end

--- Get the next client in a tab display.
-- @param tabindex The tab index.
-- @param cl The current client.
-- @return The next client.
function next(tabindex, cl)
    local c = cl or tabbed[tabindex][1]

    local i = findkey(tabbed[tabindex][2], c)

    if i == nil then
        return nil
    end

    if tabbed[tabindex][2][i+1] == nil then
        return tabbed[tabindex][2][1]
    end

    return tabbed[tabindex][2][i + 1]
end

--- Get the previous client in a tabdisplay
-- @param tabindex The tab index.
-- @param cl The current client.
-- @return The previous client.
function prev(tabindex, cl)
    local c = cl or tabbed[tabindex][1]

    local i = findkey(tabbed[tabindex][2], c)

    if i == nil then
        return nil
    end

    if tabbed[tabindex][2][i-1] == nil then
        return tabbed[tabindex][2][table.maxn(tabbed[tabindex][2])]
    end

    return tabbed[tabindex][2][i - 1]
end

--- Remove a client from a tabbed display.
-- @param cl The client to remove.
-- @return True if the client has been untabbed.
function untab(cl)
    local c = cl or capi.client.focus_get()
    local tabindex = tabindex_get(c)

    if tabindex == nil then return false end

    local cindex = findkey(tabbed[tabindex][2], c)

    if tabbed[tabindex][1] == c then
        display(tabindex, P.next(tabindex, c))
    end

    table.remove(tabbed[tabindex][2], cindex)

    if table.maxn(tabbed[tabindex][2]) == 0 then
        -- Table is empty now, remove the tabbed display
        table.remove(tabbed, tabindex)
    end

    c.hide = false
    awful.hooks.user.call('untabbed', c)
end

--- Untab all clients in a tabbed display.
-- @param tabindex The tab index.
function untab_all(tabindex)
    for i,c in pairs(tabbed[tabindex][2]) do
        c.hide = false
        awful.hooks.user.call('untabbed', c)
    end

    if tabbed[tabindex] ~= nil then
        table.remove(tabbed, tabindex)
    end
end

--- Create a new tabbed display with client as the master.
-- @param cl The client to set into the tab, focused one otherwise.
-- @return The created tab index.
function tab_create(cl)
    local c = cl or capi.client.focus_get()

    if not c then return end

    table.insert(tabbed, {
        c,    -- Window currently being displayed
        { c } -- List of windows in tabbed display
    })

    awful.hooks.user.call('tabbed', c)
    return tabindex_get(c)
end

--- Add a client to a tabbed display.
-- @param tabindex The tab index.
-- @param cl The client to add, or the focused one otherwise.
function tab(tabindex, cl)
    local c = cl or capi.client.focus_get()

    if tabbed[tabindex] ~= nil then
        local x = tabindex_get(c)

        if x == nil then
            -- Add untabbed client to tabindex
            table.insert(tabbed[tabindex][2], c)
            display(tabindex, c)

            awful.hooks.user.call('tabbed', c)
        elseif x ~= tabindex then
            -- Merge two tabbed views
            local cc = tabbed[tabindex][1]
            local clients = clients_get(x)
            untab_all(x)

            tabindex = tabindex_get(cc)

            for i,b in pairs(clients) do
                tab(tabindex, b)
            end
        end
    end
end

--- Start autotabbing, this automatically tabs new clients when the current
-- client is tabbed.
function autotab_start()
    awful.hooks.manage.register(function (c)
        local sel = capi.client.focus_get()
        local index = tabindex_get(sel)

        if index ~= nil then
            -- Currently focussed client is tabbed, 
            -- add the new window to the tabbed display
            tab(index, c)
        end
    end)
end

-- Set up hook so we don't leave lost hidden clients
awful.hooks.unmanage.register(untab)
