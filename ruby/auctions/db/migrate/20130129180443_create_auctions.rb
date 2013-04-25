class CreateAuctions < ActiveRecord::Migration

  def change
    create_table :auctions do |t|
      t.string :title
      t.text :content
      t.integer :user_id

      t.timestamps
    end
    
    add_index :auctions, [:user_id, :created_at]
  end

end
