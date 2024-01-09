"use strict";

(function(exports){

    exports.AscCrypto = exports.AscCrypto || {};
    var AscCrypto = exports.AscCrypto;

    CryptoJS.Storage = {};

    // command types
    AscCrypto.Storage.CommandType = {
        Sign  : "sign",
        Crypt : "crypt"
    };

    AscCrypto.Storage.CurrentVersion = 1;

    // Common Functions
    function randomBytes = function(count)
    {
        var array = new Uint8Array(count);
        if (window.crypto && window.crypto.getRandomValues)
            window.crypto.getRandomValues(array);
        else
        {
            for (var i = 0; i < count; i++)
                array[i] = (0x100 * Math.random()) >> 0;
        }
        return array;
    };

    function toBase58(buf)
    {
        if(typeof buf === "string")
        {
            let old = buf;
            buf = [];
            for (let i = 0, len = old.length; i < len; i++)
                buf.push(old.charCodeAt(i));
        }

        const chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
        const chars_map = [
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
            -1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
            22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
            -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
            47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        ];

        let result = [];
        for (let i = 0, len = buf.length; i < len; i++)
        {
            let carry = buf[i];
            for (let j = 0; j < result.length; ++j)
            {
                const x = (chars_map[result[j]] << 8) + carry;
                result[j] = chars.charCodeAt(x % 58);
                carry = (x / 58) >> 0;
            }
            while (carry)
            {
                result.push(chars.charCodeAt(carry % 58));
                carry = (carry / 58) >> 0;
            }
        }

        let char1 = "1".charCodeAt(0);
        for (let i = 0, len = buf.length; i < len; i++)
        {
            if (buf[i])
                break;
            else
                result.push(char1);
        }

        result.reverse();
        return String.fromCharCode.apply(null, result);
    };

    function StorageItem()
    {
        this.Id = "";
        this.Type = AscCrypto.Storage.CommandType.Sign;
        this.Date = "";

        this.Version = AscCrypto.Storage.CurrentVersion;
        this.PublicKey = "";
        this.PrivateKey = "";
        this.Salt = "";
    }

    StorageItem.prototype.generate = function(type)
    {
        let item = new StorageItem();
        item.Id   = toBase58(randomBytes(20));
        item.Type = type;

        let date = new Date();
        item.Date = date.toISOString();

        let keys = null;
        switch (type)
        {
            case AscCrypto.Storage.CommandType.Sign:
            {
                keys = window.cryptoJS.generateSignKeys();
                break;
            }
            case AscCrypto.Storage.CommandType.Crypt:
            {
                keys = window.cryptoJS.generateCryptKeys();
                break;
            }
            default:
                break;
        }

        if (keys)
        {
            item.PublicKey  = keys.publicKey;
            item.PrivateKey = keys.privateKey;
            item.Salt       = keys.salt;
        }

        return item;
    };

    function StorageLocalStorage()
    {
        
    }

    // store
    StorageLocalStorage.prototype.getStorageValue = function()
    {
        try
        {
            return JSON.parse(window.localStorage.getItem("oo-crypto-object"));
        }
        catch (e)
        {
            return [];
        }
    };
    StorageLocalStorage.prototype.setStorageValue = function(value)
    {
        try
        {
            window.localStorage.setItem("oo-crypto-object", JSON.stringify(value));
            return true;
        }
        catch (e)
        {
        }
        return false;
    };

    // commands
    StorageLocalStorage.prototype.add = function(items)
    {
        let value = this.getStorageValue();
        this.setStorageValue(value.concat(items));
    };

    StorageLocalStorage.prototype.replace = function(items)
    {
        let value = this.getStorageValue();
        let id2index = {};
        for (let i = 0, len = value.length; i < len; ++i)
            id2index[value[i].Id] = i;
        for (let i = 0, len = items.length; i < len; ++i)
        {
            let index = id2index[items[i].Id];
            if (undefined !== index)
                value[index] = items[i];
        }
    };

    StorageLocalStorage.prototype.get = function(item)
    {
        let value = this.getStorageValue();
        let result = [];

        for (let i = 0, len = value.length; i < len; ++i)
        {
            let isAdd = true;
            for (let i in item)
            {
                if (item.hasOwnProperty(i) && item[i] !== value[i])
                {
                    isAdd = false;
                    break;
                }
            }
            if (isAdd)
                result.push(value[i]);
        }

        return result;
    };

    StorageLocalStorage.prototype.getByType = function(type)
    {
        return this.get({ Type : type });
    };

    StorageLocalStorage.prototype.remove = function(id)
    {
        let value = this.getStorageValue();
        for (let i = 0, len = value.length; i < len; ++i)
        {
            if (value[i].Id === id)
            {
                value.splice(i, 1);
                break;
            }
        }
        this.setStorageValue(value);
    };

    AscCrypto.Storage.Item = StorageItem;
    AscCrypto.Storage.LocalStorage = StorageLocalStorage;

})(window);
